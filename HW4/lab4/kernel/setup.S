@ setup.S: setup the 
@
@ Author: Carlos Gil <cgil@andrew.cmu.edu>
@	  Varun Kohli <vkohli@andrew.cmu.edu>
@	  Rishabh Alaap Singh <rasingh@andrew.cmu.edu>
@ Date:   10/20/2012 3:10 pm

/*This file contains assembly that sets up all the modes, specifically
 *- Switch to user mode with IRQs & FIQs masked.
 *- Setup a full descending user mode stack (with the stack top at 0xa3000000).
 *- Push U-Boot’s argc & argv on the user stack.
 *- Jump to a loaded user program at address 0xa2000000.
 */
#include <bits/swi.h>
#include <arm/psr.h>
	
#define USER_PROG_ADDR 0xa0000000
#define IRQ_SP 0xa2000000

.global setup_user
.global exitcall
	
setup_user:
	/*store old registers/stack pointer to access later*/
	stmfd sp!, {r4-r7,r9-r11, lr}
	ldr ip, =SP_SAVED
	str sp, [ip]
	ldr ip, =R8_SAVED
	str r8, [ip]
	
	/*switch to IRQ mode, then setup the IRQ barebones*/
	mrs r2, cpsr
	bic r2, r2, #0xF
	orr r2, r2, #0x2
	msr cpsr, r2
	mov sp, #IRQ_SP

	/*Switch to USER mode */
	mrs r2, cpsr
	bic  r2, r2, #0xFF
	orr  r2, r2, #0x50 	@ cpsr = nzcvqiFt_USER
	msr cpsr, r2

	/*Set stack pointer to 0xa3000000*/
	mov sp, #0xa3000000

	/*Push NULL terminator of argv*/
	mov ip, #0
	str ip, [sp, #-4]!
	
	/*Get argc*/
	add r1, r1, r0, asl #2
	mov r2, r0 /* count = argc */
	
/*Place argc and argcv's on stack*/
.loadStack:
	cmp r2, #0
	ldrgt ip, [r1, #-4]!
	strgt ip, [sp, #-4]!
	subgt r2, r2, #1
	bgt .loadStack 	//continue loop

/*jump to user loaded program at 0xa2000000*/
.loadProgram:
	str r0, [sp, #-4]! /*store argc on the stack*/
	ldr r2, =USER_PROG_ADDR
	blx r2
	
/*our exit swi essentially*/
exitcall:	
	ldr r1, =SP_SAVED
	ldr sp, [r1]
	ldmfd sp!, {r4-r7,r9-r11, lr}
	mov pc, lr

	.comm SP_SAVED, 4, 4
	.comm R8_SAVED, 4, 4
