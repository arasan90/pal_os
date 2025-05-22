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
			timer			  = next;
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

void pal_os_timer_remove(pal_timer_t *timer)
{
	// Remove the timer from the sorted list
	pal_timer_t *prev = NULL;
	pal_timer_t *cur  = pal_timer_environment.timer_list;

	while (cur && cur != timer)
	{
		prev = cur;
		cur	 = cur->next;
	}

	if (cur)
	{
		if (prev)
		{
			prev->next = cur->next;
		}
		else
		{
			pal_timer_environment.timer_list = cur->next;
		}
	}
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

		// Check if a timer has already expired
		int already_expired = pal_os_timer_time_cmp(&timer->expiry_time, &current_time) <= 0;
		int wait_result		= 0;
		// Wait for the timer to expire or for a shutdown signal
		if (!already_expired)
		{
			wait_result = pthread_cond_timedwait(&pal_timer_environment.cond, &pal_timer_environment.mutex, &timer->expiry_time);
		}

		// If the wait timed out, check if the timer has expired
		if (already_expired || wait_result == ETIMEDOUT)
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
					pal_timer_restart(timer, 0);
				}
				else
				{
					// Remove the timer from the list if it is one-shot
					pal_os_timer_remove(timer);
					pthread_mutex_unlock(&pal_timer_environment.mutex);
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

int pal_timer_create(pal_timer_t *timer, const char *name, pal_timer_type_t type, size_t period, pal_timer_callback_t callback, int auto_start, void *arg)
{
	int ret_code = -1;
	(void)name;
	if (timer && callback && period)
	{
		timer->callback	   = callback;
		timer->arg		   = arg;
		timer->is_periodic = (type == PAL_TIMER_TYPE_PERIODIC);
		timer->period_ms   = period;
		if (auto_start)
		{
			pal_timer_start(timer, 0);
		}
		pthread_cond_signal(&pal_timer_environment.cond);
		ret_code = 0;
	}
	return ret_code;
}

int pal_timer_start(pal_timer_t *timer, int from_isr)
{
	int ret_code = -1;
	(void)from_isr;
	if (timer && !timer->is_started)
	{
		struct timespec current_time;
		clock_gettime(CLOCK_MONOTONIC, &current_time);
		timer->expiry_time.tv_sec  = current_time.tv_sec + timer->period_ms / 1000;
		timer->expiry_time.tv_nsec = current_time.tv_nsec + (timer->period_ms % 1000) * 1000000;
		if (timer->expiry_time.tv_nsec >= 1000000000)
		{
			timer->expiry_time.tv_sec++;
			timer->expiry_time.tv_nsec -= 1000000000;
		}
		pal_os_timer_insert_sorted(timer);
		timer->is_started = 1;
		pthread_cond_signal(&pal_timer_environment.cond);
		ret_code = 0;
	}
	return ret_code;
}

int pal_timer_stop(pal_timer_t *timer, int from_isr)
{
	int ret_code = -1;
	(void)from_isr;
	if (timer)
	{
		pal_os_timer_remove(timer);
		timer->is_started = 0;
		pthread_cond_signal(&pal_timer_environment.cond);
		ret_code = 0;
	}
	return ret_code;
}

int pal_timer_restart(pal_timer_t *timer, int from_isr)
{
	int ret_code = -1;
	(void)from_isr;
	if (timer)
	{
		pal_timer_stop(timer, 0);
		pal_timer_start(timer, 0);
		ret_code = 0;
	}
	return ret_code;
}

int pal_timer_change_period(pal_timer_t *timer, size_t new_period, int from_isr)
{
	int ret_code = -1;
	(void)from_isr;
	if (timer && new_period)
	{
		timer->period_ms = new_period;
		pal_timer_restart(timer, 0);
		ret_code = 0;
	}
	return ret_code;
}

int pal_is_timer_active(pal_timer_t *timer)
{
	int ret_code = 0;
	if (timer)
	{
		ret_code = timer->is_started;
	}
	return ret_code;
}

int pal_timer_delete(pal_timer_t *timer)
{
	int ret_code = -1;
	if (timer)
	{
		pal_timer_stop(timer, 0);
		return 0;
	}
	return ret_code;
}