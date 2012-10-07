//
//  rot13.c
//  
//
//  Created by Rishabh Alaap Singh on 10/7/12.
//
//

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
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
      //spaces ignore
      else if (test == 32) 
	continue;
      else
	exit(1); //Error Case
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
      bytesRead = read(0, string, 256); //256 is a number large enough to hold most input strs
      if (!bytesRead) 
	{
	  exit(0);   
	}
      output = encryptish(string, bytesRead);
      write(1, output, bytesRead);
    }
  
  exit(0);
}
