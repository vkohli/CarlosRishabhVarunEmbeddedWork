/** @file typo.c
 * Carlos gil : cgil
 * Varun Kohli : vkohli
 * Rishabh Singh : rasingh
 * @brief Echos characters back with timing data.
 *
 * Links to libc.
 */

#include <unistd.h>
#include <time.h>
#include <stdio.h>

#define BUFSIZE 100

int main(int argc, char** argv)
{
	char input[BUFSIZE];
	time_t TIMER;
	
	//Temp time variable
	time_t initialTime;
	
	//Temp final time
	int timeDiff;
	
	char c_time_string[BUFSIZE];
	int sizeRead;
	
	while(1)
	{
		//Remove this line
		TIMER = time(NULL);
		
		
		//Track time
		initialTime = TIMER;
	
		//Prompt user for input
		write(STDOUT_FILENO, "Wheres the money punk? ", 23);
		//read input
		sizeRead = read(STDIN_FILENO, input, sizeof(input));
		//Display the input back to the user
		write(STDOUT_FILENO, input, sizeRead);
		
		//Display time taken to enter input
		timeDiff = (int)((int)TIMER - (int)initialTime);
		sprintf(c_time_string, "%d\n", timeDiff );
		
		write(STDOUT_FILENO, c_time_string, sizeof(timeDiff));
		
	
	
	}
	return 0;
}
