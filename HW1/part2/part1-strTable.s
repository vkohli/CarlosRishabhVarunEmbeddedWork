	.file	"part1-strTable.c"
	.text
	.align	2
	.global	strTable
	.type	strTable, %function
strTable:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6}
	mov	r4, #0	@ swapped r4 and r2 remove a move instruction  
	cmp	r4, r3	@ swapped r5 and r0, r0 and r3, r3 and r5 to remove move instructions
	bge	.L11
.L9:
	ldrb	ip, [r1, r4]	@ zero_extendqisi2  @ remove duplicate code in L7 and useless r3
.L7:
	cmp ip, #69				@ Pseudo binary search for char
	subge ip, ip, #69
	sublt ip, ip, #23
	cmp	ip, #22
	bgt	.L7
.L13:
	cmp	ip, r2
	bgt	.L4
	ldrb	r5, [r0, ip]	@ zero_extendqisi2
	ldrb	r6, [r1, r4]	@ zero_extendqisi2
	cmp	r3, r6
	strneb	r6, [r0, ip]
.L4:
	add	r4, r4, #1
	cmp	r4, r3
	blt	.L9
.L11:
	ldmfd	sp!, {r4, r5, r6}
	mov pc, lr
	.size	strTable, .-strTable
	.ident	"GCC: (GNU) 3.4.5"