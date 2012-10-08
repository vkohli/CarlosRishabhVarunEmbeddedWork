/**
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Varun Kohli <vkohli@andrew.cmu.edu>
 *	    	Carlos Gil <cgil@andrew.cmu.edu>
 *	    	Rishabh Singh <rasingh@cmu.edu>
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
		if (input_str[i] == '\n')
		{
			break;
		}
		// first half of alphabet
		if ((test >= 'A' && test <= 'M') || (test >= 'a' && test <= 'm'))
		{
			input_str[i] += 13;
		}
		//second half of alphabet
		else if ((test >= 'N' && test <= 'Z') || (test >= 'a' && test <= 'z'))
		{
			input_str[i] -= 13;
		}
		//ignore all other characters and pass them a long 
		else 
		{
			continue;
		}
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
		if (!bytesRead) //no bytes read
		{
			exit(0);   
		}
		else if(bytesRead < 0) //error
		{
			exit(1);
		}
		output = encryptish(string, bytesRead);
		if(write(STDOUT_FILENO, output, bytesRead) < 0) //if returns less than 0 its an error
		{
				exit(1);
		}
		else	//flush the buffer
		{
			fflush(STDOUT_FILENO);
		}
	}
	
  exit(0);
}

