/* File: typo.c
 * Carlos gil : cgil
 * Varun Kohli : vkohli
 * Rishabh Alaap Singh : rasingh
 * Brief: Echos characters back with timing data about how long it took the
 * user to type the input. Works as a test case for the IRQ and SWI Handlers 
 * as it involves both read, write as well as timer.
 *
 * Links to libc.
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 100

int main(int argc, char** argv)
{
  char input[BUFSIZE];

  /*Temp time variable*/
  unsigned int initialTime;
  
  /*Temp final time*/
  unsigned long timeDiff;  
  int sizeRead;
  unsigned long decimal_val;
  
  while(1)
    {
      /*Track time*/
      initialTime = time();
      
      /*Prompt user for input*/
      write(STDOUT_FILENO, "> ", 2);

      /*read input*/
      if((sizeRead = read(STDIN_FILENO, input, sizeof(input))) < 0)
	exit(1);

      /*Display the input back to the user*/
      if(write(STDOUT_FILENO, input, sizeRead) <0 )
	exit(1);

      /*Display time taken to enter input*/
      timeDiff = ((unsigned int)time() - initialTime);
      timeDiff = (timeDiff*100)/100;

      decimal_val = (timeDiff % 1000)/100;
      timeDiff /= 1000;

      printf("%lu.%lu\n", timeDiff, decimal_val);
         
    }
  return 0;
}
