#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>

#define SWI 0x8
#define IRQ 0x18
#define LDRCONST 0xE51FF004
#define DEREF(unsigned p) (*(unsigned *)(p)) 

uint32_t global_data;
unsigned int time; /*contains time (in ms) from booting the kernel*/
void irq_handler();


int kmain(int argc, char** argv, uint32_t table)
{
  
	app_startup(); /* bss is valid after this point */
	global_data = table;	
    
	install_handler(SWI,(unsigned) &swi_handler);
	install_handler(IRQ,(unsigned) &irq_handler);

	/*
	//Install IRQ Handler
	unsigned IRQ_addr = 0x18; //Address of the IRQ Instr

	//IRQ_instr contains the instruction LDR pc, pc, offset
	unsigned IRQ_vec = *(unsigned int *)IRQ_addr;
	unsigned IRQ_instr_offset = (IRQ_vec & 0xFFF);
	unsigned IRQ_instr = (0x18 + IRQ_instr_offset + )
	*/



	/* Add your code here */
	
	uninstall_handler(SWI);
	uninstall_handler(IRQ);

	return 0;
}

/*install_handler
 *installes a custom written handler into the system. This process happens
 *twice (for SWI and for IRQ), which is why we decided to put this in 
 *a function.
 */
void install_handler(unsigned vec_addr, unsigned instr_install_addr)
{
  unsigned pc_offset = 0x8;

  /*find the instruction located at this vector address*/
  unsigned vec_instr = DEREF(vec_addr);

  /*We know that this vec_instr represents the instr "LDR pc, pc, offset".
   So we go to the actual address of the handler*/
  unsigned offset = (vec_instr & 0xFFF);
  unsigned instr_address = DEREF(vec_instr + offset + pc_offset);

  /*Save the original 2 instructions into global variables. Since we 
   are only calling this twice, we can have them stored manually*/
  if (vec_addr == SWI)
    {
      SWI_instr1 = DEREF(instr_address);
      SWI_instr2 = DEREF(instr_address + 4);
    }
  else
    {
      assert(vec_addr == IRQ);
      IRQ_instr1 = DEREF(instr_address);
      IRQ_instr2 = DEREF(instr_address + 4);
    }

  /*Now, replace the original instruction code with your code*/
  DEREF(instr_address) = LDRCONST;
  DEREF(instr_address + 4) = instr_install_addr;

  return;
}

/*uninstall_handler
 *given a manually installed handler, this function uninstalls it.
 */
void uninstall_handler(unsigned vec_addr)
{
  unsigned pc_offset = 0x8;

  unsigned vec_instr = DEREF(vec_addr);
  unsigned offset = (vec_instr & 0xFFF);
  
  unsigned instr_addr = DEREF(vec_instr + offset + pc_offset);

  if (vec_addr == SWI)
    {
      DEREF(instr_addr) = SWI_instr1;
      DEREF(instr_addr + 4) = SWI_instr2;
    }

  else
    {
      assert(vec_addr == IRQ);
      DEREF(instr_addr) = IRQ_instr1;
      DEREF(instr_addr + 4) = IRQ_instr2;
    }

  return;
}

/*strategy - for the timer, initialize OSMR to 1 millisecond, and have a setup such that the IRQ handler is called whenever OSCR = OSMR*/

/*irq_handler
 *the IRQ interrupt handler contains the implementation of kernel whenever an 
 *IRQ is called. Since in our implementation the IRQ handler is called every
 *millisecond, we will be incrementing the global var 'time' and incrementing
 *the OSMR to the next millisecond. 
 */
void irq_handler()
{
  printf("--Entered IRQ Handler--");

  /*Notes and Assumptions:
   - "reg_read" and "reg_write" are function prototypes defined in kernel/include/arm/regs.h

   - The clock speed is 3.25 MHz. Hence, each millisecond (ms) is represented
   by 3250 clock cycles

   - There exists a global uint32_t var named "time" that starts at 0

   - The OSMR is initialized to 3250 so that the first interrupt
called in the first millisecond*/

  /*Increment the time as this is executed every millisecond*/
  time++;
  printf("time = %u", time);
 
  /*Set OSMR to the next millisecond value*/
  uint32 time_osmr = reg_read(OSTMR_OSMR_ADDR);
  time_osmr += 3250;
  reg_write(OSTMR_OSMR_ADDR, time_osmr);

  /*disable the iterrupt by placing 1 into OSSR*/
  reg_write(OSTMR_OSSR_M0, 1);

  printf("--Exited IRQ Handler--");
  return;
}
