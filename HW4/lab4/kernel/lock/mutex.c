/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * @author Harry Q Bovik < PUT YOUR NAMES HERE
 *
 * 
 * @date  
 */

//#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif

mutex_t gtMutex[OS_NUM_MUTEX];

void mutex_init()
{
	/*---WRITTEN BY RISHABH---*/
	
	/*I'm just assigning 0 to unlock and 1 to lock, and -1 to unassigned. 
	If it's not this, i'm sure it's a quick fix. TAs told me this is an 
	easy part in lab.*/
	for (i = 0; i < OS_NUM_MUTEX; i++)
	{
		gtMutex[i] = 0;
	}
	
	return;
}

int mutex_create(void)
{
	/*---WRITTEN BY RISHABH---*/
	
	/*Look for the smallest index that is still a -1, assign a 0 to it.*/
	int i = 0;
	while (i < OS_NUM_MUTEX)
	{
		if (gtMutex[i] != -1)
		{
			i++;
		}
	}
	
	gtMutex[i] = 0;
	return i; 
}

int mutex_lock(int mutex  __attribute__((unused)))
{
	/*---WRITTEN BY RISHABH---*/
	
	/*Check for Errors*/
	if (!((mutex >= 0) && (mutex <= OS_NUM_MUTEX))) return EINVAL;
	if (gtMutex[mutex] == 1) return EDEADLOCK;
	
	/*Set that value in the mutex to 1.*/
	gtMutex[mutex] = 1;
	return 0; /*Return 0 on success*/
}

int mutex_unlock(int mutex  __attribute__((unused)))
{
	/*---WRITTEN BY RISHABH---*/
	
	/*Check for Errors*/
	if (!((mutex >= 0) && (mutex <= OS_NUM_MUTEX))) return EINVAL;
	if (gtMutex[mutex] == 0) return EDEADLOCK;
	
	/*Set that value in the mutex to 1.*/
	gtMutex[mutex] = 0;
	return 0; /*Return 0 on success*/
}

