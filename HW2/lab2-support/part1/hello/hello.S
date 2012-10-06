@ hello.S: Hello world! in assembly                                             
@                                                                               
@ Authors: Carlos Gil cgil@andrew.cmu.edu                                                      
@          Varun Kohli vkohli@andrew.cmu.edu    
@		   Rishabh Signh rsignh@cmu.edu                                  
@ Date:    9/5/2012 5:45pm                                             
                 
#include <bits/swi.h>   
#include <bits/fileno.h>                                                            
        .file   "hello.S"                                                       
        .text                                                                   
                                                                                
        .global _start                                                          
_start:                                                                         
        ldr r1, phello_str
        mov r0, #STDOUT_FILENO
        mov r2, #hello_size
		swi WRITE_SWI
		mov r0, #0
		swi EXIT_SWI 
        
phello_str:
	.word hello_str
	.section .rodata
	
hello_str:
	.ascii "Hello world!\n"
	.set hello_size, .-hello_str
