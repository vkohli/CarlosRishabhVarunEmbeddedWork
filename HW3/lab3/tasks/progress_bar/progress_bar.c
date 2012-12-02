/* File: progress_bar.c
 * Carlos gil : cgil
 * Varun Kohli : vkohli
 * Rishabh Singh : rasingh
 *
 * Brief: This is our own test case for the SWI and IRQ handlers. This program
 * creates a progress bar, based on input parameters of time. This is a good
 * test as it involves the write and sleep syscalls of the SWI and IRQ Handlers.
 *
 * Links to libc.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 100

int main(int argc, char** argv)
{    
  char input[BUFSIZE];
  char newInput[BUFSIZE];

  /*Temp time variable*/
  unsigned int initialTime;
  
  /*Temp final time*/
  unsigned int timeDiff;
  
  int sizeRead;
  int totalTime;
  
  int timeSoFar = 0;
  int sleepTime;
  
  write(STDOUT_FILENO, "Milliseconds to load: ", 22); /*Prompt*/
  sizeRead = read(STDIN_FILENO, input, sizeof(input));
  totalTime = (int)atoi(input);

  write(STDOUT_FILENO, "Milliseconds between bars: ", 27);
  sizeRead = read(STDIN_FILENO, newInput, sizeof(input));
  sleepTime = (int)atoi(newInput);

  write(STDOUT_FILENO, "\n", 1);

  //Track time
  initialTime = time();
  
  while(timeSoFar < totalTime)
    {
      write(STDOUT_FILENO, "|", 1);
      timeSoFar += sleepTime;
      sleep(sleepTime);
    }
  write(STDOUT_FILENO, "\n", 1);
  
  /*End timer: total time taken*/
  /*Display time taken to enter input*/
  
  /*Replace the bottom two lines*/
  timeDiff = (unsigned int)(time() - initialTime);
  printf("Time elapsed: %d\n", timeDiff);	
 
  return 0;
}
