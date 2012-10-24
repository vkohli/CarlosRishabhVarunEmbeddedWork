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
.global setup
	
setup:	
	// Switch to user mode with IRQS and FIQS masked
	MRS r2, cpsr 
	BIC  r2, r2, #0xCF 	@ cpsr = nzcvqIFt_USER
	MSR cpsr, r2
	
	@ Setup a full descending user mode stack
	mov sp, #0xa3000000

	@ Push U-Boot’s argc & argv on the user stack.
	@ APCS r0 contains argc, r1 contains argv
	stmfd sp!, {r0} 	@ push argc onto the stack
	mov r2, r0		@ get a copy of argc
	ldr r3, [r1]		@ get a reference to r1

.loadStack:
	cmp r2, #0
	ble .loadProgram	@ no more arguments to push
	ldr r4, [r3]		@ get the value stored at r3 (r3 is an index into the argv array)
	stmfd    sp!, {r4}	@ push argument to stack
	add r3, r3, #4		@ get next array value in argv
	sub r2, r2, #1		@ subtract from argument counter
	b .loadStack
	
@ Jump to a loaded user program at address 0xa2000000.
.loadProgram:
	mov pc, #0xa2000000
