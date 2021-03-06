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
#include <bits/swi.h>
	
	.file "time.S"

FUNC(time)
	stmfd sp!, {r1-r12, lr}
	swi TIME_SWI
	ldmfd sp!, {r1-r12, lr}
	mov pc, lr
