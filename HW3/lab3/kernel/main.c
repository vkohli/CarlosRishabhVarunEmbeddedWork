#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>

#include <bits/fileno.h>
#include <bits/errno.h>
#include <types.h>

#define ERRORCONST 0x0BADC0DE
#define LDRCONST 0xE51FF004
#define SDRAMLOW 0xA0000000
#define SDRAMHIGH 0xA3FFFFFF
#define ROMLOW 0x0
#define ROMHIGH 0xFFFFFF

uint32_t global_data;

void C_SWI_handler(unsigned swi_num, unsigned *regs);
int setup_user(int argc, char *argv[]);
unsigned s_handler();
ssize_t read_syscall(int fd, char* buf, int count);
ssize_t write_syscall(int fd, char* buf, int count);
unsigned int get_handler_addr(unsigned int swiVecAddr);
void exitcall(int exitStatus);

int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* bss is valid after this point */
	global_data = table;

	unsigned int swiVecAddr, swiHandAddr;
	unsigned int savedSwiOne, savedSwiTwo;
	unsigned int exitStatus;

	swiVecAddr = 0x8;

	//install the swiHandler, get the former instructions out
	swiHandAddr = get_handler_addr(swiVecAddr);

	// save old swi hanglers
	savedSwiOne = *(unsigned int*) swiHandAddr;
	savedSwiTwo = *(unsigned int*) (swiHandAddr+4);

	//install the swi handler
	*(unsigned int *) swiHandAddr = (unsigned) LDRCONST;
	*(unsigned int *) (swiHandAddr+4) = (unsigned) &s_handler;

	//install setup user stack and to user mode
	exitStatus = setup_user(argc, argv);
	
	//restore old swiHandler
	*(unsigned int*) swiHandAddr = savedSwiOne;
	*(unsigned int*) (swiHandAddr+4) = savedSwiTwo;

	return exitStatus;
}

unsigned int get_handler_addr(unsigned int swiVecAddr)
{
  unsigned int swiVecInstr;
  unsigned int registers, offset, offsetSign, opcode;

  //get instruction code in hex
  swiVecInstr = *(unsigned int *) swiVecAddr;
  
  //offset from instruction
  offset = (swiVecInstr & 0xFFF);
  offsetSign = (swiVecInstr >> 23) & 1;
  registers = ((swiVecInstr >> 12) & 0xFF);

  // opcode is storing not just the opcoe but the condition(always)
  // and the load bit as well
  opcode = (swiVecInstr >> 20) & 0xFFF;
  
  /*
   * If the registers are not pc, and the opcode does not
   * have condition always, the load/str code, and 
   * the load bit set, return an error.
   */
  if (registers != 0xFF || (opcode != 0xe51 && opcode != 0xe59))
    {
      printf("Unrecognized instruction: %u @ 0x08\n", swiVecInstr);
      return ERRORCONST;
    }

  if (!offset) offset = offset*(-1);

  // address of the swiHandler
  return *(unsigned int *)(offset+swiVecAddr+0x8);
}

/**
 * handles system_calls
 */
void C_SWI_handler(unsigned swi_num, unsigned *regs)
{
  char* buf;
  int fd;
  int count;
  int exitStatus;

  switch(swi_num) {
  //exit
  case 0x900001: 
    exitStatus = regs[0];
    exitcall(exitStatus);

  //read, check for errors, else call the helper
  case 0x900003:
    fd = regs[0];
    buf = (char *) regs[1];
    count = regs[2];

    //if the fd is not stdin
    if(fd != STDIN_FILENO)
      {
       regs[0] = -EBADF;
       break;
      }
    
    //check the range
    if ((unsigned)&buf < (unsigned)SDRAMLOW || 
	(unsigned)&buf+count > (unsigned)SDRAMHIGH)
      {
	regs[0] = -EFAULT;
	break;
      }
    regs[0] = read_syscall(fd, buf, count);
    break;

  //write, check for errors else call the helper
  case 0x900004:
    fd = regs[0];
    buf = (char *) regs[1];
    count = regs[2];
    
    //check fd
    if(fd != STDOUT_FILENO) 
      {
	regs[0] = -EBADF;
	break;
      } 
   //check range
    if(!((unsigned)&buf >= (unsigned)SDRAMLOW && 
	(unsigned) &buf+count < (unsigned) SDRAMHIGH) ||
       ((unsigned) &buf+count < (unsigned) ROMHIGH))
      {
	regs[0] = -EFAULT;
	break;
      }
    regs[0] = write_syscall(fd, buf, count);
    break;

  default:
    printf("Invalid swinumber: %d\n", swi_num);
    exitcall(ERRORCONST);
  }
  return;
}

/**
 * Checks for special chars and reads in the buffer, 
 * returns len of read buf.
 */
ssize_t read_syscall(int fd, char* buf, int count)
{
  int bytesRead = 0;
  char charRead;

  while (bytesRead < count) 
    {
      charRead = getc();
      
      //EOT
      if (charRead == 4)
	return bytesRead;
      //backspace or delete
      else if(charRead == 8 || charRead == 127)
	{
	  bytesRead--;
	  puts("\b \b");
	}
      // carriage return or newline
      else if(charRead == 10 || charRead == 13)
	{
	  buf[bytesRead] = '\n';
	  bytesRead++;
	  putc('\n');
	  return bytesRead;
	}
      else
	{
	  buf[bytesRead] = charRead;
	  bytesRead++;
	  putc(charRead);
	}
    }
  return bytesRead;
}

/**
 * Writes buf to STDOUT, returns number of chars written.
 */
ssize_t write_syscall(int fd, char* buf, int count)
{
  int byteWritten = 0;
  while(byteWritten < count) 
    {
      if(buf[byteWritten] == '\0')
	return byteWritten;
      else
	{
	  putc(buf[byteWritten]);
	  byteWritten++;
	}
    }
  return byteWritten;
}
