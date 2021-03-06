@ write.S: write sycall wrapper
@
@ Authors: Carlos Gil cgil@andrew.cmu.edu                                                      
@          Varun Kohli vkohli@andrew.cmu.edu    
@	   	   Rishabh Signh rsignh@cmu.edu   
@ Date:    9/6/2012 4:30pm

#include <bits/swi.h> 

        .file   "write.S"
        .text

        .global write
write:
	stmfd sp!, {r1-r12,lr}
    swi WRITE_SWI
    cmp r0, #0 			@check if there is an error
	mov r2, #-1
	mov r3, r0			@if there is an error, r0 will be -Errno
	mullt r0, r3, r2 	@set r0 to errno
	ldrlt r4, =errno
	strlt r0, [r4] 		@save the value of errno into the errno location
	movlt r0, #-1 		@indicate that there is an error (return -1)
	ldmfd sp!, {r1-r12, lr}
	mov pc, lr

