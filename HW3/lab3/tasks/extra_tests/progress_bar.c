/** @file progress_bar.c
 * Carlos gil : cgil
 * Varun Kohli : vkohli
 * Rishabh Singh : rasingh
 * @brief Creates a progress bar, compare time it should have taken to our perceived timer.
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
	int totalTime;

	time_t timeSoFar = 0;
	int sleepTime;
	
	write(STDOUT_FILENO, "Seconds to load: ", 17);
	sizeRead = read(STDIN_FILENO, input, sizeof(input));
	totalTime = (int)atoi(input);
	
	write(STDOUT_FILENO, "Seconds between bars: ", 22);
	sizeRead = read(STDIN_FILENO, input, sizeof(input));
	sleepTime = (int)atoi(input);
	
	//Start timer
	//Remove this line
	TIMER = time(NULL);
	//Track time
	initialTime = TIMER;
	
	while(timeSoFar < totalTime)
	{
		write(STDOUT_FILENO, "|", 1);
		timeSoFar += sleepTime;
		sleep(sleepTime);
	
	}
	write(STDOUT_FILENO, "\n", 1);
	
	//End timer: total time taken
	//Display time taken to enter input
	
	//Replace the bottom two lines
	//timeDiff = (int)((int)TIMER - (int)initialTime);
	timeDiff = (int)((int)time(NULL) - (int)initialTime);
	sprintf(c_time_string, "%ds\n", timeDiff );	
	write(STDOUT_FILENO, c_time_string, sizeof(timeDiff));
	
	return 0;
}