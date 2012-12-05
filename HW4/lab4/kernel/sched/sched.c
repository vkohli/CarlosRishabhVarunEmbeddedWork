/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <config.h>
#include "sched_i.h"

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */

void sched_init(task_t* main_task  __attribute__((unused)))
{
	/*Not needed. I believe I updated the scheduler within task_create- Rishabh*/
}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
 
static void __attribute__((unused)) idle(void)
{
	 enable_interrupts();
	 while(1);
}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
void allocate_tasks(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	/*---WRITTEN BY RISHABH---*/
	/*What this does is assigns a priority for each TCB in system_tcb, 
	and from this point, for an index i, system_tcb[i] represents tasks[i] in the kernel*/
	
	/*Sanity check on the number of tasks that we have to allocate*/
	assert(num_tasks < OS_MAX_TASKS);
	
	/*Set up TCBs for all tasks by assigning priorities. Because we
	placed the tasks in ascending order of C, we know that system_tcb[i] represents
	tasks[i] of the user.*/
	/*Put the task in the runqueue (make it available for running)*/
	for (i = 0; i < num_tasks; i++) /*NOTE: num_tasks may be lesser than OS_MAX_TASKS, so there can be free uninitialized TCBs*/
	{
		system_tcb[i]->native_prio = i; /*Priority 0 makes no sense??*/
		system_tcb[i]->cur_prio = i;
		system_tcb[i]->context.r4 = 0;
		system_tcb[i]->context.r5 = 0;
		system_tcb[i]->context.r6 = 0;
		system_tcb[i]->context.r7 = 0;
		system_tcb[i]->context.r8 = 0;
		system_tcb[i]->context.r9 = 0;
		system_tcb[i]->context.r10 = 0;
		system_tcb[i]->context.r11 = 0;
		system_tcb[i]->context.sp = (*tasks[i].stack_pos);
		system_tcb[i]->context.lr = NULL; /*idk..?*/

		runqueue_add(system_tcb[i], i);
	}
	
	/*Set up the TCB for the idle task --> done above when i = 63, right?*/
	
	/*make the idle task schedulable --> idk what they're trying to say here*/
}

