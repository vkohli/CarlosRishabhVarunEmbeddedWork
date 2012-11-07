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

int main(int argc, char** argv)
{
	char input[100];
	time_t TIMER;
	
	//Temp time variable
	time_t initialTime;
	
	//Temp final time
	time_t timeDiff;
	
	while(1)
	{
		//Remove this line
		TIMER = time(NULL);
		
		
		//Track time
		initialTime = TIMER;
	
		//Prompt user for input
		write(STDOUT_FILENO, "Wheres the money punk? ", 23);
		//read input
		read(STDIN_FILENO, input, 100);
		//Display the input back to the user
		write(STDOUT_FILENO, input, sizeof(input) - 1);
		
		//Display time taken to enter input
		timeDiff = TIMER - initialTime;
		write(STDOUT_FILENO, timeDiff, 50);
		
	
	
	}
	return 0;
}
