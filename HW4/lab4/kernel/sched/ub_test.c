/** @file ub_test.c
 * 
 * @brief The UB Test for basic schedulability
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

//#define DEBUG 0

#include <sched.h>
#ifdef DEBUG
#include <exports.h>
#endif

 
/**
 * @brief Perform UB Test and reorder the task list.
 *
 * The task list at the end of this method will be sorted in order is priority
 * -- from highest priority (lowest priority number) to lowest priority
 * (highest priority number).
 *
 * @param tasks  An array of task pointers containing the task set to schedule.
 * @param num_tasks  The number of tasks in the array.
 *
 * @return 0  The test failed.
 * @return 1  Test succeeded.  The tasks are now in order.
 */
int assign_schedule(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	//Lookup for U(k) = 2^(1/k) - 1 
	long u[12];
	u[0] = -1;
	u[1] = 1;
	u[2] = 0.4142;
	u[3] = 0.2599;
	u[4] = 0.1892;
	u[5] = 0.1486;
	u[6] = 0.1224;
	u[7] = 0.10408;
	u[8] = 0.0905;
	u[9] = 0.08;
	u[10] = 0.071;
	u[11] = 0.065;
	
	//U(k)
	long uk;
	
	int i = 0;
	
	
	//Get U(k)
	if ( num_tasks < 12 )
	{
		uk = u[num_tasks];
		
	}
	else
	{
		uk = u[11];
	}
	//No tasks
	if (uk < 0)
	{
		return 0;
	}
	
	int cTotal = 0;
	unsigned long time = 0;
	for(i = 0; i < num_tasks; i++)
	{
		time = (tasks[i].C + tasks[i].B)/tasks[i].T;
		time += cTotal;
		if (uk > time)
		{
			return 0;
		}
		else
		{
			cTotal += tasks[i].C / tasks[i].T;
		}
		
	}
	

	return 1; //Success
}
	


