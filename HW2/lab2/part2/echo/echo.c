/** @file echo.c
 *
 * @author  Carlos Gil : cgil
 *			Varun Kohli : vkohli
 *			Rishabh Singh : rsingh
 * @date    10/20/2012
 */

#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv) {

  write(1, "FUCK THIS SHIT\n", 15);
  return 0;
}
