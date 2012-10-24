/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Varun Kohli <vkohli@andrew.cmu.edu>
 *         Carlos Gil <cgil@andrew.cmu.edu>
 *         Rishabh Singh <rasingh@andrew.cmu.edu>
 *
 * Date:   10/17/12
 */
#import <exports.h>
#import <bits/fileno.h>
#import <bits/errno.h>

#define ERRORCONST 0x0BADC0DE
#define LDRCONST 0xE51FF004
#define SDRAMLOW 0xA0000000
#define SDRAMHIGH 0xA3FFFFFF
#define ROMLOW 0x0
#define ROMHIGH 0x00FFFFFF

void C_SWI_handler(unsigned swi_num, unsigned *regs);
void setup(int argc, char *argv[]);
unsigned s_handler();
int check_range(unsigned int addr, int count, unsigned int low, unsigned int high);
ssize_t read_syscall(int fd, char* buf, int count);

int main(int argc, char *argv[]) {

  unsigned int swiVecInstr;
  unsigned int swiHandAddr;
  unsigned int swiVecAddr;
  unsigned int registers, offset, offsetSign, opcode;
  unsigned int savedInstrOne, savedInstrTwo;

  // location
  swiVecAddr = 0x8;
  swiVecInstr = *(unsigned int *) swiVecAddr;
  
  offset = (swiVecInstr & 0xFFF);
  registers = ((swiVecInstr >> 12) & 0xFF);
  offsetSign = (swiVecInstr >> 23) & 1;

  // opcode is storing not just the opcode, but
  // the condition (always) and the load bit as well
  opcode = (swiVecInstr >> 20) & (0xFFF);

  /*
   * If the registers are not pc, and the opcode does not
   * have condition always, the load/str code, and 
   * the load bit set, return an error. Also if the offset
   * is set to be negative.
   */
  if (registers != 0xFF || !offsetSign ||
      (opcode != 0xe51 && opcode != 0xe59)) 
    {
      printf("Unrecognized instruction: %u @ 0x08\n", swiVecInstr);
      return ERRORCONST;
    }

  // this is the address of the swiHandler
  swiHandAddr = *(unsigned int *)(offset+swiVecAddr+0x8);
  
  //save the next two words because we're gonna overwrite them
  savedInstrOne = *(unsigned int*) swiHandAddr;
  savedInstrTwo = *(unsigned int*) (swiHandAddr+4);
  
  // install our handler
  *(unsigned int *)swiHandAddr = (unsigned) LDRCONST;
  *(unsigned int *)(swiHandAddr+4) = (unsigned) &s_handler;
  printf("saddr: %u/sinstr: %u\n", *(unsigned int *) swiHandAddr,
	 *(unsigned int *) (swiHandAddr+4));

  // call stack/usermode setup function
  setup(argc-1, &argv[1]);

  //restore old swiHandler
  *(unsigned int*) swiHandAddr = savedInstrOne;
  *(unsigned int*) (swiHandAddr+4) = savedInstrTwo;
  printf("saddr: %u/sinstr: %u\n", *(unsigned int *) swiHandAddr,
	 *(unsigned int *) (swiHandAddr+4));
  return 0;
}


void C_SWI_handler(unsigned swi_num, unsigned *regs)
{
  char* buf;
  int fd;
  int count;

  switch(swi_num) {
  //exit
  case 0x900001: 
    printf("exit!\n");
    return;

  //read
  case 0x900003:
    printf("read!\n");
    fd = regs[0];
    buf = (char *) regs[1];
    count = regs[2];

    //if the fd is not stdin
    if(fd != STDIN_FILENO)
      {
       regs[0] = -EBADF;
       return;
      }
    
    //check the range
    if (!check_range((unsigned int)&buf, count, (unsigned) SDRAMLOW, (unsigned) SDRAMHIGH))
      {
	regs[0] = -EBADF;
	return;
      }

    regs[0] = read_syscall(fd, buf, count);
    return;

  //write
  case 0x900004:
    printf("write\n");
    regs[0] = 0;
    break;

  default:
    printf("Invalid swinumber: %d\n", swi_num);
    regs[0] = ERRORCONST;
    return;
  }
}

int check_range(unsigned int addr, int count, unsigned int low, unsigned int high) 
{
  if((addr < low || addr > high) ||
     ((addr+count) < low || (addr+count) > high)) return 0;
  else return 1;
}

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