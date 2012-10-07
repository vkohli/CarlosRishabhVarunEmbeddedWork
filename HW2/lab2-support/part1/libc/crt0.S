@ crt0.S: Execution startup routine
@
@ Authors: Group Member 1 <email address>
@          Group Member 2 <email address>
@ Date:    The current time & date

	.file	"crt0.S"
	.text

	.global	_start
_start:
	r0 -> argc
	sp+4 -> r1
	store return address r14
	call main
	r0 -> return value from main
	exit
