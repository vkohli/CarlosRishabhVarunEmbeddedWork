@ hello.S: Hello world! in assembly                                             
@                                                                               
@ Authors: Carlos Gil cgil@andrew.cmu.edu                                                      
@          Varun Kohli vkohli@andrew.cmu.edu    
@	   Rishabh Alaap Signh rishabh@cmu.edu
@ Date:    9/5/2012 5:45pm                                             
                 
#include <bits/swi.h>   
#include <bits/fileno.h>                                                            
        .file   "hello.S"                                                       
        .text                                                                   
                                                                                
        .global _start                                                          
_start:                                                                         
        ldr r1, phello_str		@ load the string
        mov r0, #STDOUT_FILENO		@ get the file descriptor 
        mov r2, #hello_size		@ get string length
	swi WRITE_SWI			@ cal handler
	mov r0, #0			@ set the return value
	swi EXIT_SWI 			@ exit
        
phello_str:
	.word hello_str
	.section .rodata
	
hello_str:
	.ascii "Hello world!\n"		@ the string
	.set hello_size, .-hello_str
