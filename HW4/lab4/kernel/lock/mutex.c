/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * @author Carlos Gil : cgil
 * @author Varun Kohli : vkohli
 * @author Rishabh Singh : rasingh
 *
 * 
 * @date  12/7/12 5:07 pm
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
	//Initiate everything in gtMutex to zero
	int i = 0;
	for (i = 0; i < OS_NUM_MUTEX; i++)
	{
		gtMutex[i].bAvailable = 0;
		gtMutex[i].pHolding_Tcb = 0;
		gtMutex[i].bLock = 0;
		gtMutex[i].pSleep_queue = 0;
	}
	
	return;
	
}

int mutex_create(void)
{
	//Loop through all identifiers and finds an available mutex
	int i = 0;
	for (i = 0; i < OS_NUM_MUTEX; i++)
	{
		//It's available
		if (gtMutex[i].bAvailable == 0)
		{
			gtMutex[i].bAvailable = 1;
			return i;
		}
	}
	
	//None available return the error
	return ENOMEM;

}

int mutex_lock(int mutex  __attribute__((unused)))
{
	int i = 0;
	tcb_t* current_tcb = get_cur_tcb();
	tcb_t* holding_tcb;
	
	/*Check for Errors*/
	if (!((mutex >= 0) && (mutex < OS_NUM_MUTEX)))
	{
		return EINVAL;
	}
	//Check if already holding task
	holding_tcb = gtMutex[mutex].pHolding_Tcb;
	if ( current_tcb == holding_tcb)
	{
		return EDEADLOCK;
	}
	//if not locked then acquire it
	if (gtMutex[mutex].bLock == 0 )
	{
		gtMutex[mutex].bLock = 1;
		gtMutex[mutex]->pHolding_Tcb = current_tcb;
	}
	else //It's locked, set to sleep until free
	{
		dispatch_sleep();
	}

	return 0; /*Return 0 on success*/
}

int mutex_unlock(int mutex  __attribute__((unused)))
{
	tcb_t* current_tcb = get_cur_tcb();
	
	/*Check for Errors*/
	if (!((mutex >= 0) && (mutex < OS_NUM_MUTEX)))
	{
		return EINVAL;
	}
	//Current task does not hold the mutex
	if ( !(gtMutex[mutex].pHolding_Tcb == current_tcb) ) 
	{
		return EDEADLOCK;
	}
	//Give possession to next thing in sleep queue or unlock if empty
	if ( gtMutex[mutex].pSleep_queue == 0 ) //Empty queue so unlock
	{
		gtMutex[mutex].bLock = 0;
	}
	else //Someone is waiting for this mutex; give it to next person in queue
	{
		//Set holding tcb to be the head of whats in the sleep queue
		gtMutex[mutex].pHolding_Tcb = gtMutex[mutex].pSleep_queue.sleep_queue;
		//Move the sleep queue forward to the next thing in the queue ... should I set this to 0 if empty?
		gtMutex[mutex].pSleep_queue.sleep_queue = gtMutex[mutex].pSleep_queue->sleep_queue;
	}
	
	return 0; /*Return 0 on success*/
}

