/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * Group -
	Varun Kohli (vkohli@andrew.cmu.edu)
	Carlos Gil (cgil@andrew.cmu.edu)
	Rishabh Alaap Singh (rasingh@andrew.cmu.edu)
 * @date 2008-10-31
 */

#include <asm.h>

	.file "time.S"

FUNC(time)
	stmfd sp!, {r1-r12, lr}
	swi TIME_SWI //the include file says this SWI is not linux compatible
	//Now, we have our results, so pull from stack
	ldmfd sp!, {r1-r12, lr}
	mov pc, lr
