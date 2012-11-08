/** @file typo.c
 * Carlos gil : cgil
 * Varun Kohli : vkohli
 * Rishabh Singh : rasingh
 * @brief Echos characters back with timing data.
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
  //Temp time variable
  unsigned int initialTime;
  
  //Temp final time
  unsigned int timeDiff;  
  float output;
  int sizeRead;
  
  while(1)
    {
      //Track time
      initialTime = time();
      
      //Prompt user for input
      write(STDOUT_FILENO, "> ", 2);
      //read input
      if((sizeRead = read(STDIN_FILENO, input, sizeof(input))) < 0)
	exit(1);
      //Display the input back to the user
      if(write(STDOUT_FILENO, input, sizeRead) <0 )
	exit(1);

      //Display time taken to enter input
      timeDiff = ((unsigned int)time() - initialTime);
      timeDiff = (timeDiff*100)/100;
      output = (timeDiff/1000.0);
      printf("%u\n", timeDiff);
      
      //      write(STDOUT_FILENO, c_time_string, sizeof(timeDiff));    
    }
  return 0;
}
