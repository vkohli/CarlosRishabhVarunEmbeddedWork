@ crt0.S: Execution startup routine
@
@ Authors: Carlos Gil <cgil@andrew.cmu.edu>
@          Varun Kohli <vkohli@andrew.cmu.edu>
@          Rishabh Alaap Singh <rishabh@cmu.edu>
@
@ Date:    The current time & date

#include <swi.h>

	.file	"crt0.S"
	.text
	
	.global	_start
_start:	
	mov r0, [sp]
	mov r1, [sp, #4]@dereference the first argv value, we can use the address to find the other argv values
	@store return address r14
	str r14, [sp]
	@call main
	b   main
	@r0 -> return value from main
	@exit
	b exit
	