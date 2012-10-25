/** @file echo.c
 *
 * @author  Carlos Gil : cgil
 *	    Varun Kohli : vkohli
 *	    Rishabh Singh : rasingh
 * @date    10/20/2012
 */

#include <unistd.h>
#include <stdlib.h>
#include <bits/fileno.h>
#include <bits/types.h>

int main(int argc, char** argv) {
  int count, fd;
  char* buf;
  int i;

  fd = STDOUT_FILENO;
  count = 256;

  for (i = 0; i < argc; i++)
    {
      buf = argv[i];
      if (write(fd, buf, count) < 0) 
	exit(1);
    }
  return 0;
}
