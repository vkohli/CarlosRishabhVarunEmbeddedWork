/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-12
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>

int task_create(task_t* tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
  	/*---WRITTEN BY RISHABH---*/
	
	/*verify that all tasks are schedulable*/
	/*NOTE: Please change to appropriate error numbers, as mentioned in the API*/

	if (tasks == NULL) return -1;
	if ((num_tasks <= 0) && (num_tasks > 64)) return -1;
	
	for (i = 0; i < num_tasks; i++)
	{
		if (tasks[i]->lambda == NULL) return -1;
		if (tasks[i]->data == NULL) return -1;
		if (tasks[i]->stack_pos == NULL) return -1;
		if (tasks[i]->C <= 0) return -1;
		if (tasks[i]->T <= 0) return -1;
		if (tasks[i]->C > tasks[i]->T) return -1;
	}
	
	/*PART2: Schedulability SHOULD go here*/

	/*Put them into ascending order of timing by comparing C values*/
	/*NOTE: THIS NEEDS TO CHANGE TO LIKE BUBBLE SORT OR SOMETHING*/
	void *temp_task = malloc(sizeof(task_t));
	for (j = 0; j < num_tasks; j++)
	{
		for (k = 0; k < num_tasks; k++)
		{
			if ((tasks[j]->C) > (tasks[k]->C))
			{
				/*swap*/
				temp = tasks[j];
				tasks[j] = tasks[k];
				tasks[k] = temp;
			}
		}
	}


	/*schedule the tasks*/
	allocate_tasks(tasks, num_tasks);
}

int event_wait(unsigned int dev  __attribute__((unused)))
{
	/*---WRITTEN BY RISHABH---*/
	/*Event_wait*/
	 /*Checks*/
	assert ((dev >= 0) && (dev <= 3)); /* replace with
	code to return EINVAL on error */
	
	/*call dev_wait*/
	dev_wait(dev);
	
	/*call dispatch_sleep()*/
	dispatch_sleep();
	
  return 0; /*0 for success*/
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
