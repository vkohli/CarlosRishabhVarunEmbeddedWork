/** @file ctx_switch_asm.S
 *
 * @brief Contains the core context switch routine.
 *
 * These routines all assume that the caller is following AAPCS, needs
 * no co-processor registers and that the caller is in SVC mode. Furthermore,
 * the SPSR is considered a spare/scratch register and hence, is not saved.
 *
 * @author Kartik Subramanian
 * @date   2008-11-21
 */

	.file "ctx_switch_asm.S"

#include <asm.h>
#include <arm/psr.h>

/**
 * @brief Special exit routine from the scheduler that launches a task for the
 * first time.
 *
 * r4 contains the user entry point.
 * r5 contains the single argument to the user function called.
 * r6 contains the user-mode stack pointer.
 * Upon completion, we should be in user mode.
 */
FUNC(launch_task)
	mov     r0, r5
	mov     r1, #0
	mov     r2, #0
	mov     r3, #0
	mov     r5, #0
	mov     r8, #0
	mov     ip, #0
	msr     cpsr_c, #(PSR_MODE_USR)
	mov     sp, r6
	mov     r6, #0
	ldr     lr, =0xdeadbeef   /* Causes a crash instead of calling the reset vector */
	mov     pc, r4

/* r0 points to the target context, r1 to the current context. */
/* add your code to perform a full context switch */
FUNC(ctx_switch_full)
/*---WRITTEN BY RISHABH---*/
/*Read ctx_switch_half first to understand what's happening. What we do is first copy over values from regs to TCB_cur, and then copy over values from TCB_high into regs*/
	
	/*Store values*/
	str	r4, r1
	str	r5, r1, #32
	str	r6, r1, #64
	str	r7, r1, #96
	str	r8, r1, #128
	str	r9, r1, #160
	str	r10, r1, #192
	str	r11, r1, #224
	str	sp, r1, #256 
	str	lr, r1, #320

	/*Load values*/
	ldr	r4, r0
	ldr	r5, r0, #32
	ldr	r6, r0, #64
	ldr	r7, r0, #96
	ldr	r8, r0, #128
	ldr	r9, r0, #160
	ldr	r10, r0, #192
	ldr	r11, r0, #224
	ldr	sp, r0, #256 
	ldr	lr, r0, #320

	mov pc, lr /*This continues the program of the defined task from where it had left off*/



/*add your code to perform a half context switch */	
FUNC(ctx_switch_half)
/*---WRITTEN BY RISHABH---*/
/*We are given the addr to the r4 value in the context in the TCB, which is
stored in r0 based on how functions work. What we need to do is copy over the context into the registers*/
	ldr	r4, r0
	ldr	r5, r0, #32
	ldr	r6, r0, #64
	ldr	r7, r0, #96
	ldr	r8, r0, #128
	ldr	r9, r0, #160
	ldr	r10, r0, #192
	ldr	r11, r0, #224
	ldr	sp, r0, #256 
	ldr	lr, r0, #320

	mov pc, lr /*This continues the program of the defined task from where it had left off*/

