# vkohli, cgil, rasingh
# Goodbye.s - edited assembly of hello.s
	.file	"hello.c"
	.section	.rodata
	.align	2
.LC0:
	.ascii	"Hello world!\000"
	.align 	2
.LC1:
	.ascii	"Goodbye world!\000"
  	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	ldr	r0, .L3
	bl	puts
	ldr	r0, .L3+4
	bl	puts
	mov	r0, #42
	bl	exit
.L4:
	.align	2
.L3:
	.word	.LC0
	.word	.LC1
	.size	main, .-main
	.ident	"GCC: (GNU) 4.1.2"
