/** @file run_queue.c
 * 
 * @brief Run queue maintainence routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <sched.h>
#include "sched_i.h"



static tcb_t* run_list[OS_MAX_TASKS]  __attribute__((unused));

/* A high bit in this bitmap means that the task whose priority is
 * equal to the bit number of the high bit is runnable.
 */
static uint8_t run_bits[OS_MAX_TASKS/8] __attribute__((unused));

/* This is a trie structure.  Tasks are grouped in groups of 8.  If any task
 * in a particular group is runnable, the corresponding group flag is set.
 * Since we can only have 64 possible tasks, a single byte can represent the
 * run bits of all 8 groups.
 */
static uint8_t group_run_bits __attribute__((unused));

/* This unmap table finds the bit position of the lowest bit in a given byte
 * Useful for doing reverse lookup.
 */
static uint8_t prio_unmap_table[]  __attribute__((unused)) =
{

0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

/**
 * @brief Clears the run-queues and sets them all to empty.
 */
void runqueue_init(void)
{
	/*---WRITTEN BY RISHABH---*/
	
	/*run_list is an array of pointers to TCBs. so we set these pointers*/
	/*NOTE: This may have to be done in runqueue_add*/
	for (i = 0; i < OS_MAX_TASKS; i++)
	{
		*run_list[i] = system_tcb[i]; /*We get the system_tcb array from sched.c somehow*/
	}
	
	group_run_bits = 0; /*group_run_bits == 0 => by definition that the entire array in run_bits has value 0*/
	
	return;
}

/**
 * @brief Adds the thread identified by the given TCB to the runqueue at
 * a given priority.
 *
 * The native priority of the thread need not be the specified priority.  The
 * only requirement is that the run queue for that priority is empty.  This
 * function needs to be externally synchronized.
 */
void runqueue_add(tcb_t* tcb  __attribute__((unused)), uint8_t prio  __attribute__((unused)))
{
	/*---WRITTEN BY RISHABH---*/
	
	/*We need to update the group_run_bits and run_bits*/
	uint8_t row = prio/8;
	uint8_t col = prio % 8;
	uint8_t i = 8*row + col;
	
	group_run_bits = (group_run_bits | 1 << row);
	run_bits[i] = 1;
	
	/*idk why we need the tcb here..? Maybe I am not allowed to 
	allocate all the pointers in init and I should do it here. If I do,
	here is the code:*/
	run_list[i] = tcb;
	
	return;
}


/**
 * @brief Empty the run queue of the given priority.
 *
 * @return  The tcb at enqueued at the given priority.
 *
 * This function needs to be externally synchronized.
 */
tcb_t* runqueue_remove(uint8_t prio  __attribute__((unused)))
{
	void *pop; /*Statically declared so shouldn't cause malloc-related issues*/
	
	/*---WRITTEN BY RISHABH---*/
	
	uint8_t row = prio/8;
	uint8_t col = prio % 8;
	uint8_t i = 8*row + col;
	
	/*Simple sanity check that group_run_bits show that there does exist some elem in the row in the run list*/
	assert(((group_run_bits >> row) && 0x01) == 1);
	
	/*Set the coordinate to 0*/
	uint8_t mask_col = ~(1 << col);
	run_bits[row] = (run_bits[row] & mask_col); 
	
	/*If setting this coordinate to 0 makes the whole row 0, then set 
	that row to 0 as well*/
	if (run_bits[row] == 0)
	{
		uint8_t mask_row = ~(1 << row);
		group_run_bits = (group_run_bits & mask_row);
	}
	
	/*Set the run_list ptr of the respective coordinate to 0*/
	/*NOTE: Not sure if we have to do this*/
	pop = run_list[i];
	run_list[i] == NULL;
	
	return (pop);	
}

/**
 * @brief This function examines the run bits and the run queue and returns the
 * priority of the runnable task with the highest priority (lower number).
 */
uint8_t highest_prio(void)
{
	/*---WRITTEN BY RISHABH---*/
	
	/*This is given in the Lab4 Handout Page 9 so I'm assuming this is right*/
	uint8_t y = prio_unmap_table[group_run_bits];
	uint8_t x = prio_unmap_table[run_bits[y]];
	uint8_t prio = (y << 3) + x;
	
	return prio;	
}
