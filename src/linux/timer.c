/*
 * File: timer.c
 * Description: Implementation of timer-related functionality for the Linux platform.
 * Author: Massimiliano Ianniello
 */

#include "pal_os/timer.h"

#include <errno.h>
#include <malloc.h>
#include <pthread.h>

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
 * Variables
 * ---------------------------------------------------------------------------
 */
pal_timer_env_t pal_timer_environment = {
	.thread_handle = 0,
	.mutex		   = PTHREAD_MUTEX_INITIALIZER,
	.cond		   = PTHREAD_COND_INITIALIZER,
	.timer_list	   = NULL,
	.shutdown_flag = 0,
};

/* ---------------------------------------------------------------------------
 * Static Functions
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Function Implementations
 * ---------------------------------------------------------------------------
 */
int pal_timer_init(void) { return (0 == pthread_create(&pal_timer_environment.thread_handle, NULL, pal_timer_thread_fn, NULL)) ? 0 : -1; }

void pal_timer_deinit(void)
{
	if (pal_timer_environment.thread_handle)
	{
		pthread_mutex_lock(&pal_timer_environment.mutex);
		pal_timer_environment.shutdown_flag = 1;
		pthread_cond_signal(&pal_timer_environment.cond);
		pthread_mutex_unlock(&pal_timer_environment.mutex);
		pthread_join(pal_timer_environment.thread_handle, NULL);
		pal_timer_environment.thread_handle = 0;
		pal_timer_t *timer					= pal_timer_environment.timer_list;
		while (timer)
		{
			pal_timer_t *next = timer->next;
			free(timer);
			timer = next;
		}
		pal_timer_environment.timer_list	= NULL;
		pal_timer_environment.shutdown_flag = 0;
	}
}

int pal_os_timer_time_cmp(const struct timespec *a, const struct timespec *b)
{
	int result = 0;
	if (a->tv_sec == b->tv_sec)
	{
		result = a->tv_nsec - b->tv_nsec;
	}
	else
	{
		result = a->tv_sec - b->tv_sec;
	}
	return result;
}

void pal_os_timer_insert_sorted(pal_timer_t *timer)
{
	// Insert the timer into the sorted list based on its expiry time
	pal_timer_t *prev = NULL;
	pal_timer_t *cur  = pal_timer_environment.timer_list;

	while (cur && pal_os_timer_time_cmp(&cur->expiry_time, &timer->expiry_time) < 0)
	{
		prev = cur;
		cur	 = cur->next;
	}

	if (prev)
	{
		prev->next = timer;
	}
	else
	{
		pal_timer_environment.timer_list = timer;
	}

	timer->next = cur;
}

void *pal_timer_thread_fn(void *arg)
{
	(void)arg;

	while (1)
	{
		pthread_mutex_lock(&pal_timer_environment.mutex);

		// Check if the timer list is empty or if the shutdown flag is set
		while (NULL == pal_timer_environment.timer_list && !pal_timer_environment.shutdown_flag)
		{
			pthread_cond_wait(&pal_timer_environment.cond, &pal_timer_environment.mutex);
		}

		// Kill the thread if the shutdown flag is set
		if (pal_timer_environment.shutdown_flag)
		{
			pthread_mutex_unlock(&pal_timer_environment.mutex);
			break;
		}

		struct timespec current_time;
		clock_gettime(CLOCK_MONOTONIC, &current_time);

		// Check the first timer in the list (the one that expires first)
		pal_timer_t *timer = pal_timer_environment.timer_list;

		// Wait for the timer to expire or for a shutdown signal
		int wait_result = pthread_cond_timedwait(&pal_timer_environment.cond, &pal_timer_environment.mutex, &timer->expiry_time);

		// If the wait timed out, check if the timer has expired
		if (wait_result == ETIMEDOUT)
		{
			// Execute timer callback if the timer has expired
			if (pal_os_timer_time_cmp(&timer->expiry_time, &current_time) <= 0)
			{
				// Call the timer callback function
				timer->callback(timer->arg);

				// If the timer is periodic, reset its expiry time and reinsert it into the list
				if (timer->is_periodic)
				{
					// Add the period to the expiry time
					timer->expiry_time.tv_sec += timer->period_ms / 1000;
					timer->expiry_time.tv_nsec += (timer->period_ms % 1000) * 1000000;
					if (timer->expiry_time.tv_nsec >= 1000000000)
					{
						timer->expiry_time.tv_sec++;
						timer->expiry_time.tv_nsec -= 1000000000;
					}
					pal_timer_environment.timer_list = timer->next;
					pal_os_timer_insert_sorted(timer);
				}
				else
				{
					// Remove the timer from the list and free resources if it is one-shot
					pal_timer_environment.timer_list = timer->next;
					free(timer);
					pthread_mutex_unlock(&pal_timer_environment.mutex);
					break;
				}
			}
		}
		else
		{
			// If the wait was interrupted, check if the shutdown flag is set
			if (pal_timer_environment.shutdown_flag)
			{
				pthread_mutex_unlock(&pal_timer_environment.mutex);
				break;
			}
		}
		pthread_mutex_unlock(&pal_timer_environment.mutex);
	}
	return NULL;
}

int pal_timer_create(pal_timer_t **timer, pal_timer_type_t type, size_t period, pal_timer_callback_t callback, int auto_start, void *arg)
{
	int ret_code = -1;
	if (timer && callback && period)
	{
		*timer = (pal_timer_t *)calloc(1, sizeof(pal_timer_t));
		if (*timer)
		{
			(*timer)->callback	  = callback;
			(*timer)->arg		  = arg;
			(*timer)->is_periodic = (type == PAL_TIMER_TYPE_PERIODIC);
			(*timer)->period_ms	  = period;
			struct timespec current_time;
			clock_gettime(CLOCK_MONOTONIC, &current_time);
			(*timer)->expiry_time.tv_sec  = current_time.tv_sec + period / 1000;
			(*timer)->expiry_time.tv_nsec = current_time.tv_nsec + (period % 1000) * 1000000;
			if ((*timer)->expiry_time.tv_nsec >= 1000000000)
			{
				(*timer)->expiry_time.tv_sec++;
				(*timer)->expiry_time.tv_nsec -= 1000000000;
			}
			if (auto_start)
			{
				pal_os_timer_insert_sorted(*timer);
			}
			pthread_cond_signal(&pal_timer_environment.cond);
			ret_code = 0;
		}
	}
	return ret_code;
}

int pal_timer_start(pal_timer_t *timer)
{
	int ret_code = -1;
	if (timer)
	{
		pal_os_timer_insert_sorted(timer);
		pthread_cond_signal(&pal_timer_environment.cond);
		ret_code = 0;
	}
	return ret_code;
}