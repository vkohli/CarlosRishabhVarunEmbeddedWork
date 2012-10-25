@ read.S: read sycall wrapper
@
@ Authors: Carlos Gil cgil@andrew.cmu.edu                                                      
@          Varun Kohli vkohli@andrew.cmu.edu    
@	   Rishabh Signh rasignh@cmu.edu   
@ Date:    9/6/2012 4:30pm

#include <bits/swi.h> 

        .file   "read.S"
        .text

        .global read
read:
	stmfd sp!, {r1-r12, lr} 		@ Push r1 - r4 into the registers (callee saved)
	swi READ_SWI
	cmp r0, #0 			@ Check if there is an error from the implementation
	mov r2, #-1
	mov r3, r0
	mullt r0, r3, r2 		@ If there is an error, the libc returns the errno.
	ldrlt r4, =errno 		@ Set the pointer of errno into r8
	strlt r0, [r4] 			@ Push the errno from libc into the errno location
	movlt r0, #-1 			@ set return status to -1 as there is an error
	ldmfd sp!, {r1-r12,lr} 		@ restore previous values
	mov pc, lr

