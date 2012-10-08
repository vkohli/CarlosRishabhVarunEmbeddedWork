@ crt0.S: Execution startup routine
@
@ Authors: Carlos Gil <cgil@andrew.cmu.edu>
@          Varun Kohli <vkohli@andrew.cmu.edu>
@          Rishabh Alaap Singh <rishabh@cmu.edu>
@
@ Date:    9/7/2012	9:30pm

	.file	"crt0.S"
	.text
	
	.global	_start
_start:	
	ldr r0, [sp]
	ldr r1, [sp, #4]			@ dereference the first argv value, we can use the address to find the other argv values
	str r14, [sp]				@ store return address r14
	b   main					@ r0 -> return value from main
	b exit


	
