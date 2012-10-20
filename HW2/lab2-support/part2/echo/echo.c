/** @file echo.c
 *
 * @author  Carlos Gil : cgil
 *			Varun Kohli : vkohli
 *			Rishabh Singh : rsingh
 * @date    10/20/2012
 */

#include <exports.h>

int main(int argc, char** argv) {

	if ( argc > 0 ) {
		puts(argv[1]);
	}

	return 0;
}
