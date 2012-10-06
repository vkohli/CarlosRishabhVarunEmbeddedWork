@ exit.S: exit sycall wrapper
@
@ Authors: Carlos Gil cgil@andrew.cmu.edu                                                      
@          Varun Kohli vkohli@andrew.cmu.edu    
@		   Rishabh Signh rsignh@cmu.edu   
@ Date:    9/5/2012 6:30pm
#include <bits/swi.h> 
        .file   "exit.S"
        .text

        .global exit
exit:
        swi EXIT_SWI
        cmp r0, #0
		mov r9, #-1
		mov r10, r0
		mullt r0, r10, r9
		ldrlt r8, =errno
		strlt r0, [r8]
		movlt r0, #-1
		mov pc, lr
