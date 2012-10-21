@ crt0.S: Execution startup routine
@
@ Authors: Carlos Gil <cgil@andrew.cmu.edu>
@          Varun Kohli <vkohli@andrew.cmu.edu>
@          Rishabh Alaap Singh <rishabh@cmu.edu>
@
@ Date:    The current time & date

#include <swi.h>
#include <main.c>

	.file	"crt0.S"
	.text

	.global	_start
_start:
	@r0 -> argc
    mov r0, [sp]

    @sp+4 -> r1
    mov r8, sp, #4
    mov r1, [r8] @dereference the first argv value, we can use the address to find the other argv values

	@store return address r14
    str r14, [sp]

	@call main
    b   main

	@r0 -> return value from main

    @exit
    swi EXIT_SWI