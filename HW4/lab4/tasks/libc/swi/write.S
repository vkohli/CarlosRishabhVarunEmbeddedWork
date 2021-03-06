/** @file write.S
 *
 * @brief write sycall wrapper
 *
 * @author Varun Kohli <vkohli@andrew.cmu.edu>
 * 		Carlos Gil <cgil@andrew.cmu.edu>
 *		Rishabh Singh <rasingh@cmu.edu>
 * @date   Sun, 07 Oct 2007 01:53:46 -0400
 */

#include <asm.h>
#include <bits/swi.h>
	
	.file "write.S"

FUNC(write)
	stmfd sp!, {r1-r12, lr}
	swi WRITE_SWI
	cmp r0, #0
	mov r2, #-1
	mov r3, r0
	mullt r0, r3, r2
	ldrlt r4, =errno
	strlt r0, [r4]
	movlt r0, #-1
	ldmfd sp!, {r1-r12,lr}
	mov pc, lr
