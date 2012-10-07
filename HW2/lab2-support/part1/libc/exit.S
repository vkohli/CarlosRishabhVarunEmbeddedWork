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
		stmfd !sp, {r4-r11, r14}
        swi EXIT_SWI
        ldmfd	sp!, {r4-r11, r14}
