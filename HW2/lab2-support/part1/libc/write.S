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
        cmp r0, #0
		mov r9, #-1
		mov r10, r0
		mullt r0, r10, r9
		ldrlt r8, =errno
		strlt r0, [r0]
		movlt r0, #-1
		mov pc, lr