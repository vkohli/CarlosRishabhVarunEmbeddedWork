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

char *encrypt(char *input_str, int input_str_len)
{

    for (i = 0; i < input_str_len; i++)
    {
        
        if (input_str[i] == '') //end of input string
        {
            break;
        }
        
        if ((65 <= input_str[i] <= 77) || (97 <= input_str[i] <= 109))
        {
            input_str[i] += 13;
        }
        
        else if ((78 <= input_str[i] <= 90) || (110 <= input_str[i] <= 122))
        {
            input_str[i] -= 13;
        }
        
        else
        {
            exit(1); //Error Case
        }
    }
    
    
    return input_str;
}

int main()
{
    char string[256];
    int bytesRead;
    
    while (1) //infinite loop for multiple inputs
    {
        bytesRead = read(0, string, 256); //256 is a number large enough to hold most input strs
        
        if (!bytesRead) exit(0);
        
        string = encrypt(string, bytesRead);
        
        write(1, string, 256);
    }
    
    exit(0);
}
