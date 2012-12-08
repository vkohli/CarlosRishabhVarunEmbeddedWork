/** @file crt0.S
 *
 * @brief Execution startup routine
 *
 * @author Varun Kohli <vkohli@andrew.cmu.edu>
 *		Carlos Gil <cgil@andrew.cmu.edu>
 *		Rishabh Singh <rasingh@cmu.edu>
 *
 * @date   11/1/12
 */

#include <asm.h>

	.file "crt0.S"

FUNC(_start)
	ldr r0, [sp]
	add r1, sp, #4
	bl  main
	b   exit
