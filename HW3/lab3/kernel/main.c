#include <exports.h>

#include <arm/psr.h>
#include <arm/reg.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>

#include <bits/fileno.h>
#include <bits/errno.h>
#include <types.h>

/*==MACROS==*/
#define ERRORCONST 0x0BADC0DE
#define LDRCONST 0xE51FF004 /*Represents the instruction LDR pc, [pc, -4]*/
#define SDRAMLOW 0xA0000000
#define SDRAMHIGH 0xA3FFFFFF
#define ROMLOW 0x0
#define ROMHIGH 0xFFFFFF
#define SWIADDR 0x08 /*Address of SWI in Vector Table*/
#define IRQADDR 0x18 /*Address of IRQ in Vector Table*/
/*==========*/

/*==GLOBAL VARS==*/
uint32_t global_data;
volatile unsigned long system_time; /*system_time represents the timer of
 the OS since it's booting. It is volatile as it can be changed by an 
interrupt handler, so we cannot assume it to be a constant value.*/
/*===============*/

/*==PROTOTYPE FUNCTIONS==*/

/*Handler Functions*/
unsigned s_handler(); 
unsigned irq_handler(); 
void C_IRQ_handler();
void C_SWI_handler(unsigned swi_num, unsigned *regs);
unsigned int get_handler_addr(unsigned int swiVecAddr);
int setup_user(int argc, char *argv[]);

/*SysCall Functions*/
ssize_t read_syscall(int fd, char* buf, int count);
ssize_t write_syscall(int fd, char* buf, int count);
void exitcall(int exitStatus);
void sleep_syscall(unsigned long millis);

/*=======================*/

/*kmain
 *This is the kernel's main function, which comes straight from the UBoot 
 *from start.S. We install the SWI and IRQ handlers in this function, 
 *and set up the user and IRQ stacks.
 */
int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* bss is valid after this point */
	global_data = table;

	unsigned int swiHandAddr, irqHandAddr;
	unsigned int savedSwiOne, savedSwiTwo, savedIrqOne, savedIrqTwo;
	unsigned savedICMR, savedICLR, savedOIER;
	unsigned int exitStatus;
	
	/*==Install the Handlers==*/
	
	/*Find the addresses of the SWI and IRQ handlers as defined by
	  the Gumstix*/
	swiHandAddr = get_handler_addr(SWIADDR);
	irqHandAddr = get_handler_addr(IRQADDR);

	/*Save the first 2 instructions of the old SWI and IRQ handlers 
	  (we will be overwriting this)*/
	savedSwiOne = *(unsigned int*) swiHandAddr;
	savedSwiTwo = *(unsigned int*) (swiHandAddr+4);
	savedIrqOne = *(unsigned int*) irqHandAddr;
	savedIrqTwo = *(unsigned int*) (irqHandAddr+4);

	/*Push in our own s_handler and irq_handler in the positions of 
	  the original handlers*/ 
	*(unsigned int *) swiHandAddr = (unsigned) LDRCONST;
	*(unsigned int *) (swiHandAddr+4) = (unsigned) &s_handler;
	*(unsigned int *) irqHandAddr = (unsigned) LDRCONST;
	*(unsigned int *) (irqHandAddr+4) = (unsigned) &irq_handler;

	/*========================*/

	/*Setup IRQ and OS timer registers*/
	system_time = 0;

	/*save current register values to restore later*/
	savedICMR = reg_read(INT_ICMR_ADDR);
	savedICLR = reg_read(INT_ICLR_ADDR);
	savedOIER = reg_read(OSTMR_OIER_ADDR);

	/*set ICMR, and ICLR (everythin goes to IRQ and only M0 triggers)*/
	reg_write(INT_ICMR_ADDR, 1 << INT_OSTMR_0);
	reg_write(INT_ICLR_ADDR, 0x0);

	/*Set the OSMR Register*/ 
	reg_clear(OSTMR_OIER_ADDR, 
		  OSTMR_OIER_E1 | OSTMR_OIER_E2 | OSTMR_OIER_E3);
	reg_set(OSTMR_OIER_ADDR, OSTMR_OIER_E0);
	reg_write(OSTMR_OSMR_ADDR(0), 
		  reg_read(OSTMR_OSCR_ADDR)+3250);

	/*Install setup user stack, irq stack, and to user mode*/
	exitStatus = setup_user(argc, argv);
	
	//restore old register values
	reg_write(INT_ICMR_ADDR, savedICMR);
	reg_write(INT_ICLR_ADDR, savedICLR);
	reg_write(OSTMR_OIER_ADDR, savedOIER);

	//restore old swiHandler, irq
	*(unsigned int*) swiHandAddr = savedSwiOne;
	*(unsigned int*) (swiHandAddr+4) = savedSwiTwo;
	*(unsigned int*) irqHandAddr = savedIrqOne;
	*(unsigned int*) (irqHandAddr + 4) = savedIrqTwo;

	return exitStatus;
}

/*get_handler_addr
 *This function takes in a vector address (address from the vector table) and
 *returns the address location of this interrupt's handler. Used to 
 *detect the address of the SWI and IRQ handlers, that we hence use to 
 *install our own.
 */
unsigned int get_handler_addr(unsigned int VecAddr)
{
  unsigned int VecInstr;
  unsigned int registers, offset, offsetSign, opcode;
  unsigned int pc_offset = 0x8; /*offset of the program counter*/

  /*Find the Instruction code in hex*/
  VecInstr = *(unsigned int *) VecAddr;
  
  /* Note the offset mentioned in the instruction at that location*/
  offset = (VecInstr & 0xFFF);
  offsetSign = (VecInstr >> 23) & 1;
  registers = ((VecInstr >> 12) & 0xFF);

  /*opcode is storing not just the opcoe but the condition(always)
    and the load bit as well*/
  opcode = (VecInstr >> 20) & 0xFFF;
  
  /* If the registers are not pc, and the opcode does not
    have condition always, the load/str code, and 
    the load bit set, return an error.
   */
  if (registers != 0xFF || (opcode != 0xe51 && opcode != 0xe59))
    {
      printf("Unrecognized instruction: %u @ 0x08\n", VecInstr);
      return ERRORCONST;
    }

  if (!offset) offset = offset*(-1);

  /* return the  address of the respective handler */
  return *(unsigned int *)(offset+VecAddr+pc_offset);
}

/* C_SWI_handler
 * handles system_calls for SWI instructions. Implemented by a giant 
 *switch statement for all the SWI Numbers.
 */
void C_SWI_handler(unsigned swi_num, unsigned *regs)
{
  char* buf;
  int fd;
  int count;
  int exitStatus;
  unsigned long millis;

  switch(swi_num) {

    /*Exit Syscall*/
  case 0x900001: 
    exitStatus = regs[0];
    exitcall(exitStatus);

    /*Read Syscall, check for errors, else call the helper*/
  case 0x900003:
    fd = regs[0];
    buf = (char *) regs[1];
    count = regs[2];

    /*if the fd is not stdin*/
    if(fd != STDIN_FILENO)
      {
       regs[0] = -EBADF;
       break;
      }
    
    /*check the range*/
    if ((unsigned)&buf < (unsigned)SDRAMLOW || 
	(unsigned)&buf+count > (unsigned)SDRAMHIGH)
      {
	regs[0] = -EFAULT;
	break;
      }
    regs[0] = read_syscall(fd, buf, count);
    break;

    /*Write Syscall, check for errors else call the helper */
  case 0x900004:
    fd = regs[0];
    buf = (char *) regs[1];
    count = regs[2];
 
    /*check fd*/
    if(fd != STDOUT_FILENO) 
      {
	regs[0] = -EBADF;
	break;
      } 
    /*check range*/
    if(!((unsigned)&buf >= (unsigned)SDRAMLOW && 
	(unsigned) &buf+count < (unsigned) SDRAMHIGH) ||
       ((unsigned) &buf+count < (unsigned) ROMHIGH))
      {
	regs[0] = -EFAULT;
	break;
      }
    regs[0] = write_syscall(fd, buf, count);
    break;

    /*Time Syscall*/
  case 0x900006:
    regs[0] = (unsigned long) system_time;
    break;

    /*Sleep Syscall*/
  case 0x900007:
    millis = regs[0];
    sleep_syscall(millis);
    break;

  default:
    printf("Invalid swinumber: %d\n", swi_num);
    exitcall(ERRORCONST);
  }
  return;
}

/*==SYSCALL FUNCTIONS==*/

/*read_syscall
 *Checks for special chars and reads in the buffer, 
 *returns len of read buf.
 */
ssize_t read_syscall(int fd, char* buf, int count)
{
  int bytesRead = 0;
  char charRead;
  while (bytesRead < count) 
    {
      charRead = getc();
      /*EOT*/
      if (charRead == 4)
	return bytesRead;
      /*backspace or delete*/
      else if(charRead == 8 || charRead == 127)
	{
	  bytesRead--;
	  puts("\b \b");
	}
      /*carriage return or newline*/
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

/*write_syscall
 *Writes buf to STDOUT, returns number of chars written.
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

/*sleep_syscall
 *implements a sleep routine via a while loop that does nothing.
 */
void sleep_syscall(unsigned long millis)
{
  unsigned long tempSysTime = system_time + millis;
  while(system_time < tempSysTime);
  return;
}

/*irq_handler
 *the IRQ interrupt handler contains the implementation of kernel whenever an 
 *IRQ is called. Since in our implementation the IRQ handler is called every
 *millisecond, we will be incrementing the global var 'system_time' and 
 *incrementing the OSMR to the next 10th millisecond. 
 */
void C_IRQ_handler()
{
  /*Notes and Assumptions:
<<<<<<< HEAD
   - "reg_read" and "reg_write" are function prototypes defined in kernel/include/arm/regs.h

   - The clock speed is 3.25 MHz. Hence, each millisecond (ms) is represented
   by 3250 clock cycles

   - There exists a global uint32_t var named "time" that starts at 0

   - The OSMR is initialized to 3250 so that the first interrupt
   called in the first millisecond*/

  /*Increment the time as this is executed every millisecond*/
  system_time++;
  printf("system_time = %d\n", system_time);
  // if (system_time % 1000 == 0) printf("time: %u\n", system_time);

  /*Set OSMR to the next millisecond value*/
  // printf("Hello: %u\n", reg_read(OSTMR_OSMR_ADDR(0)));
  reg_write(OSTMR_OSMR_ADDR(0), 
	    reg_read(OSTMR_OSMR_ADDR(0))+3250);
  // printf("Hello: %u\n", reg_read(OSTMR_OSMR_ADDR(0)));

  /*disable the interrupt by placing 1 into OSSR*/
  reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);
=======
   - The clock speed is 3.25 MHz. Hence, timer resolution of 10 milliseconds 
   is represented by 32500 clock cycles
   */
>>>>>>> bf87f3906a3aeef2ec17983afda5933b817bafe4

  //only do it if interrupt is set and for a correct (m0) timer interrupt
  if(reg_read(OSTMR_OSSR_ADDR) & OSTMR_OSSR_M0
     && reg_read(INT_ICPR_ADDR) >> INT_OSTMR_0)
    {
      /*Increment the time as this is executed every 10th  millisecond*/
      system_time += 10;
      
      /*Set OSMR to the next 10th millisecond value*/
      reg_write(OSTMR_OSMR_ADDR(0), 
		reg_read(OSTMR_OSMR_ADDR(0))+32500);
      
      /*disable the interrupt by placing 1 into OSSR*/
      reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);
    }
  return;
}
