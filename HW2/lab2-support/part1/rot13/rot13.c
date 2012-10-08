/**
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Varun Kohli <vkohli@andrew.cmu.edu>
 *	    Carlos Gil <cgil@andrew.cmu.edu>
 *	    Rishabh Singh <rasingh@cmu.edu>
 *
 * Date: 10/6/12 at 8:35
 */

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

char *encryptish(char *input_str, ssize_t input_str_len)
{
  int i;
  int test;

  for (i = 0; i < input_str_len; i++)
  {
	  test = (int)input_str[i];
	  //end of input string, in this case a newline
	  if (input_str[i] == 10)
		  break;
      // first half of alphabet
      if ((test >= 65 && test <= 77) || (test >= 97 && test <= 109))
		  input_str[i] += 13;
      //second half of alphabet
	  else if ((test >= 78 && test <= 90) || (test >= 110 && test <= 122))
		  input_str[i] -= 13;
      //spaces ignore and ignore backspaces
	  else 
		continue;
  }
  return input_str;
}

int main()
{
  char string[256];
  ssize_t bytesRead;
  char* output;

  while (1) //infinite loop for multiple inputs
    {
		bytesRead = read(STDIN_FILENO, string, 256); //256 is a number large enough to hold most input strs	
		if (!bytesRead) 
		{
			exit(0);   
		}
		else if(bytesRead < 0) 
		{
			exit(1);
		}
		output = encryptish(string, bytesRead);
		if(write(STDOUT_FILENO, output, bytesRead) < 0) 
		{
				exit(1);
		}
	}
  exit(0);
}

