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
  ssize_t output;
  int size = 32;
  char buf[size];
  //  int i;
  
  /*  puts("argc: %d\n", argc);
  for (i = 0; i < argc; i++) 
    {
      printf("argv[%d] = %s\n", i, argv[i]);
      }*/
  output = read(STDIN_FILENO, buf, size);
  // printf(buf);
  return output;
  
}
