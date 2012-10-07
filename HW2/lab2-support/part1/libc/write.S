@ write.S: write sycall wrapper
@
@ Authors: Carlos Gil cgil@andrew.cmu.edu                                                      
@          Varun Kohli vkohli@andrew.cmu.edu    
@		   Rishabh Signh rsignh@cmu.edu   
@ Date:    9/6/2012 4:30pm
#include <bits/swi.h> 
        .file   "write.S"
        .text

        .global read
write:
        swi WRITE_SWI
        cmp r0, #0 @check if there is an error
		mov r9, #-1
		mov r10, r0
        @if there is an error, r0 will be -Errno
		mullt r0, r10, r9 @set r0 to errno
		ldrlt r8, =errno
		strlt r0, [r8] @save the value of errno into the errno location
		movlt r0, #-1 @indicate that there is an error (return -1)
		mov pc, lr