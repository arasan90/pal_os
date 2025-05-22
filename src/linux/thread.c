/*
 * File: thread.c
 * Description: Implementation of thread-related functionality for the Linux platform.
 * Author: Massimiliano Ianniello
 */

#include <pthread.h>  // POSIX threads for thread management
#include <stdint.h>	  // For SIZE_MAX
#include <stdlib.h>	  // For malloc and free
#include <string.h>	  // For strlen

#include "thread_priv.h"  // Include the private header file
#include "timer_priv.h"
/* ---------------------------------------------------------------------------
 * Type Definitions
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Static Definitions
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Macros
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Constants
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Static Functions
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Function Implementations
 * ---------------------------------------------------------------------------
 */
int pal_thread_create(pal_thread_t *thread, const pal_thread_priority_t priority, size_t stack_size, const pal_thread_func_t func, const char *name,
					  void *const arg)
{
	int ret_code = -1;
	if (NULL != thread && NULL != func && 0 != stack_size)
	{
		thread->func	   = func;
		thread->arg		   = arg;
		thread->stack_size = stack_size;
		thread->priority   = priority;
		thread->name	   = name;
		thread->state	   = PAL_THREAD_STATE_STOPPED;

		pthread_attr_t attr;
		pthread_attr_init(&attr);
		if (stack_size < PTHREAD_STACK_MIN)
		{
			stack_size = PTHREAD_STACK_MIN;
		}
		pthread_attr_setstacksize(&attr, stack_size);

		ret_code = pthread_create(&thread->thread, &attr, pal_thread_generic_func, thread);
		if (ret_code == 0)
		{
			thread->state = PAL_THREAD_STATE_RUNNING;
		}
		pthread_attr_destroy(&attr);
	}
	return ret_code;
}

void pal_thread_sleep(const size_t time_ms)
{
	struct timespec ts;
	ts.tv_sec  = time_ms / 1000;
	ts.tv_nsec = (time_ms % 1000) * 1000000;
	nanosleep(&ts, NULL);
}

void pal_thread_scheduler_start(void)
{
	while (1)
	{
		pal_timer_init();
		// In a real implementation, this would block execution until all threads are terminated.
		pal_thread_sleep(SIZE_MAX);	 // Sleep indefinitely
	}
}

void pal_thread_join(pal_thread_t *const thread)
{
	pthread_join(thread->thread, NULL);
	thread->state = PAL_THREAD_STATE_TERMINATED;
}

const char *pal_thread_get_name(pal_thread_t *const thread) { return thread->name; }

size_t pal_thread_get_stack_watermark(pal_thread_t *const thread)
{
	(void)thread;
	return 0;
}

void pal_thread_free(pal_thread_t *thread) { (void)thread; }

void *pal_thread_generic_func(void *const arg)
{
	pal_thread_t *thread = (pal_thread_t *)arg;
	thread->func(thread->arg);
	return NULL;
}