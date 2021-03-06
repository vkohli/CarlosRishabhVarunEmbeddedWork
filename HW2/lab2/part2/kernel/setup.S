@ setup.S: setup the 
@
@ Author: Carlos Gil <cgil@andrew.cmu.edu>
@	  Varun Kohli <vkohli@andrew.cmu.edu>
@	  Rishabh Singh <rasingh@cmu.edu>
@ Date:   10/20/2012 3:10 pm

/*
 * Switch to user mode with IRQs & FIQs masked.
 * Setup a full descending user mode stack (with the stack top at 0xa3000000).
 * Push U-Boot’s argc & argv on the user stack.
 * Jump to a loaded user program at address 0xa2000000.
 */
#include <bits/swi.h>

#define SP_SAVED 0xa0000000
#define USER_PROG_ADDR 0xa2000000
	
.global setup
.global exitcall
	
setup:
	//store old registers/stack pointer to access later
	stmfd sp!, {r4-r11, lr}
	mov ip, #SP_SAVED
	str sp, [ip]
	
	//switch to usermode
	mrs r2, cpsr
	bic  r2, r2, #0x1F
	orr  r2, r2, #0xD0 	@ cpsr = nzcvqIFt_USER
	msr cpsr, r2

	//set stack pointer to 0xa3000000
	mov sp, #0xa3000000

	//push null terminator of argv
	mov ip, #0
	str ip, [sp, #-4]!
	
	//get argc
	add r1, r1, r0, asl #2
	mov r2, r0 // count = argc
	
//place argc and argcv's on stack
/*
 * TO be honest I'm not sure if this works as intended (the stack loading)
 */
.loadStack:
	cmp r2, #0
	ldrgt ip, [r1, #-4]!
	strgt ip, [sp, #-4]!
	subgt r2, r2, #1
	bgt .loadStack 	//continue loop

//jump to user loaded program at 0xa2000000
.loadProgram:
	str r0, [sp, #-4]! //store argc on the stack
	ldr r2, =USER_PROG_ADDR
	blx r2
//	swi EXIT_SWI
	
//our exit swi essentially
exitcall:	
	mov r1, #SP_SAVED
	ldr sp, [r1]
	ldmfd sp!, {r4-r11, lr}
	mov pc, lr
