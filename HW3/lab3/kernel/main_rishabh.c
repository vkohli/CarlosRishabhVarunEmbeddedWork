#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>

uint32_t global_data;
unsigned int time; /*contains time (in ms) from booting the kernel*/

int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* bss is valid after this point */
	global_data = table;

	/*Install IRQ Handler*/
	unsigned IRQ_addr = 0x18; //Address of the IRQ Instr
	unsigned IRQ_instr = *(unsigned int *)IRQ_addr;
	



	/* Add your code here */

	return 0;
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
