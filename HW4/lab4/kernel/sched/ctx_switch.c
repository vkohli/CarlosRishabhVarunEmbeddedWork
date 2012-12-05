/** @file ctx_switch.c
 * 
 * @brief C wrappers around assembly context switch routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 */
 

#include <types.h>
#include <assert.h>

#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

/*RISHABH's MACROS*/
#define IDLE_PRIORITY	63
#define OVERHEAD	16 /*Overhead == sizeof 2*(uint8_t), which is the overhead you need to pass through given a TCB addr to get to the context.*/
/*----------------*/

/*This static var tells you what the cur task is. Notice that every
function that could possibly alter the current task is all in this file - Rishabh*/
static __attribute__((unused)) tcb_t* cur_tcb; /* use this if needed */

/**
 * @brief Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 */
void dispatch_init(tcb_t* idle __attribute__((unused)))
{
	/*---WRITTEN BY RISHABH---*/
	
	/* "Set the initialization thread's priority to IDLE" */
	
	idle->native_prio = IDLE_PRIO; /*I initialized the first task's prio to 0.. */
	/*Apparently IDLE_PRIO is defined somewhere. If it's not, you can just change it
	to IDLE_PRIORITY that I defined on top*/
	return;
}


/**
 * @brief Context switch to the highest priority task while saving off the 
 * current task state.
 *
 * This function needs to be externally synchronized.
 * We could be switching from the idle task.  The priority searcher has been tuned
 * to return IDLE_PRIO for a completely empty run_queue case.
 */
void dispatch_save(void)
{
	/*---WRITTEN BY RISHABH---*/
	/*Saves and switches. so the asm function takes in 2 arguments, and the
	switching over of values first happens from live regs to context of TCB 
	that is being switched out, then from TCB of task switching in to live regs*/
	
	int8_t cur_prio = get_cur_prio(void);
	void *tcb_cur = run_list[cur_prio];
	
	int8_t high_prio = highest_prio(void);
	void *tcb_high = run_list[high_prio];
	
	/*Update the new current priority task*/
	runqueue_add(cur_tcb); /*because dispatch_save is called when a task
	is preempted by a higher priority task.*/
	cur_tcb = tcb_high;
	runqueue_remove(cur_tcb);
	
	if (tcb_high->sleep_queue == -1) /*If the task has never been run before*/
	{
		launch_task(tcb_high); /*Not sure if I'm supposed to put in an argument*/
	}
	
	/*Perform the context switch. In the asm, r0 contains high_prio TCB, and 
	r1 contains cur_prio TCB*/
	ctx_switch_full(tcb_high + OVERHEAD, tcb_cur + OVERHEAD);
}

/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)
{
	/*---WRITTEN BY RISHABH---*/
	/*dispatch_nosave copies over the context (ie regs) from the TCB of the
	task to be entered into the registers for processing*/
	
	
	uint8_t high = highest_prio(void);
	void *tcb_high = run_list[high];
	
	/*Update current priority*/
	cur_tcb = tcb_high;
	runqueue_remove(cur_tcb);
	
	if (tcb_high->sleep_queue == -1) /*If the task has never been run before*/
	{
		launch_task(tcb_high); /*Not sure if I'm supposed to put in an argument*/
	}
	
	ctx_switch_half(tcb_high + OVERHEAD); /*We're putting the pointer into r0. GOTO assembly file*/
}


/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)
{
	/*---WRITTEN BY RISHABH---*/
	/**/
	
	int8_t cur_prio = get_cur_prio(void);
	void *tcb_cur = run_list[cur_prio];
	
	int8_t high_prio = highest_prio(void);
	void *tcb_high = run_list[high_prio];
	
	/*Update the new current priority task*/
	runqueue_add(cur_tcb); /*because dispatch_save is called when a task
	is preempted by a higher priority task.*/
	cur_tcb = tcb_high;
	runqueue_remove(cur_tcb); /*Unnecessary, because the dev_wait() calls it as well... but keep in mind if there are bugs*/
	
	if (tcb_high->sleep_queue == -1) /*If the task has never been run before*/
	{
		launch_task(tcb_high); /*Not sure if I'm supposed to put in an argument*/
	}
	
	/*Perform the context switch. In the asm, r0 contains high_prio TCB, and 
	r1 contains cur_prio TCB*/
	ctx_switch_full(tcb_high + OVERHEAD, tcb_cur + OVERHEAD);
}

/**
 * @brief Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
	/*---WRITTEN BY RISHABH---*/
	/*We can easily get this from cur_tcb.*/
	
	return cur_tcb->cur_prio; 

/**
 * @brief Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{
	/*---WRITTEN BY RISHABH---*/
	
	
	/*We're using a static global variable that we will simply return here.
	This is a good idea because we can manage our current_prio throughout
	this page then, but if functions in other files need the current priority
	they have access to this function.*/
	
	return cur_tcb;
}



void add_to_sleep_queue(tcb)
{
	/*Search sleep_queue until end*/
	
	
	while (tcb_i->sleep_queue != NULL)
	{
		tcb_i = tcb->sleep_queue;
	}
	
	/*append new*/
	
}