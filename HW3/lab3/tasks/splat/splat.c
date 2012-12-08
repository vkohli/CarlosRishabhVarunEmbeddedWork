/*File: splat.c
 * Carlos Gil : cgil
 * Varun Kohli : vkohli
 * Rishabh Singh : rasingh
 * Brief: Displays a spinning cursor. Used as a test case for the IRQ Handler
 * as it requires both read and write functions in conjunction with timer 
 * functions.
 *
 * Links to libc.
 */

#include <unistd.h>


int main(int argc, char** argv)
{
  /* Creates a cursor by writing and deleting the characters |/-\ in a loop.*/
  while(1)
    {
      write(STDOUT_FILENO, "|", 1);
      sleep(200);
      write(STDOUT_FILENO, "\b \b", 3);
      
      write(STDOUT_FILENO, "/", 1);
      sleep(200);
      write(STDOUT_FILENO, "\b \b", 3);
      
      write(STDOUT_FILENO, "-", 1);
      sleep(200);
      write(STDOUT_FILENO, "\b \b", 3);
      
      write(STDOUT_FILENO, "\\", 1);
      sleep(200);
      write(STDOUT_FILENO, "\b \b", 3);
      
    }
  return 0;
}
