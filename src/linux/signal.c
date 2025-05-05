/*
 * File: signals.c
 * Description: Implementation of signals-related functionality for the Linux platform.
 * Author: Massimiliano Ianniello
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "pal_os/common.h"
#include "signal_priv.h"

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
int pal_signal_create(pal_signal_t **signal)
{
	int ret_code = -1;
	if (NULL != signal)
	{
		*signal = (pal_signal_t *)calloc(1, sizeof(pal_signal_t));
		if (NULL != *signal)
		{
			pthread_mutex_init(&(*signal)->mutex, NULL);
			pthread_cond_init(&(*signal)->cond, NULL);
			(*signal)->signals = 0;
			ret_code		   = 0;
		}
	}
	return ret_code;
}

pal_signal_ret_code_t pal_signal_wait(pal_signal_t *signal, size_t mask, size_t *received_signals, int clear_mask, int wait_all, size_t timeout_ms)
{
	pal_signal_ret_code_t ret_code = PAL_SIGNAL_FAILURE;
	if (NULL != signal && NULL != received_signals)
	{
		int				wait_condition = wait_all ? (mask == (signal->signals & mask)) : (signal->signals & mask);
		struct timespec ts;
		if (PAL_OS_INFINITE_TIMEOUT != timeout_ms)
		{
			struct timeval now;
			gettimeofday(&now, NULL);
			ts.tv_sec  = now.tv_sec + timeout_ms / 1000;
			ts.tv_nsec = (now.tv_usec * 1000) + ((timeout_ms % 1000) * 1000000);
			if (ts.tv_nsec >= 1000000000)
			{
				ts.tv_sec += ts.tv_nsec / 1000000000;
				ts.tv_nsec %= 1000000000;
			}
		}
		pthread_mutex_lock(&signal->mutex);
		switch (timeout_ms)
		{
			case PAL_OS_NO_TIMEOUT:
				if (wait_all)
				{
					ret_code = (mask == (signal->signals & mask)) ? PAL_SIGNAL_SUCCESS : PAL_SIGNAL_TIMEOUT;
				}
				else
				{
					ret_code = (signal->signals & mask) ? PAL_SIGNAL_SUCCESS : PAL_SIGNAL_TIMEOUT;
				}
				break;
			case PAL_OS_INFINITE_TIMEOUT:
				while (!wait_condition)
				{
					pthread_cond_wait(&signal->cond, &signal->mutex);
					wait_condition = wait_all ? (mask == (signal->signals & mask)) : (signal->signals & mask);
				}
				ret_code = PAL_SIGNAL_SUCCESS;
				break;
			default:
				ret_code = PAL_SIGNAL_SUCCESS;
				while (!wait_condition)
				{
					int err = pthread_cond_timedwait(&signal->cond, &signal->mutex, &ts);
					if (0 != err)
					{
						ret_code = PAL_SIGNAL_FAILURE;
						if (ETIMEDOUT == err)
						{
							ret_code = PAL_SIGNAL_TIMEOUT;
						}
						break;
					}
					wait_condition = wait_all ? (mask == (signal->signals & mask)) : (signal->signals & mask);
				}
				break;
		}
		pthread_mutex_unlock(&signal->mutex);
		*received_signals = signal->signals & mask;
		if (clear_mask && PAL_SIGNAL_SUCCESS == ret_code)
		{
			signal->signals &= ~(*received_signals);
		}
	}
	return ret_code;
}

int pal_signal_set(pal_signal_t *signal, size_t mask)
{
	int ret_code = -1;
	if (NULL != signal)
	{
		pthread_mutex_lock(&signal->mutex);
		signal->signals |= mask;
		if (0 == pthread_cond_signal(&signal->cond))
		{
			ret_code = 0;
		}
		pthread_mutex_unlock(&signal->mutex);
	}
	return ret_code;
}

int pal_signal_clear(pal_signal_t *signal, size_t mask)
{
	int ret_code = -1;
	if (NULL != signal)
	{
		pthread_mutex_lock(&signal->mutex);
		signal->signals &= ~mask;
		pthread_mutex_unlock(&signal->mutex);
		ret_code = 0;
	}
	return ret_code;
}

int pal_signal_destroy(pal_signal_t **signal)
{
	int ret_code = -1;
	if (NULL != signal && NULL != *signal)
	{
		pthread_mutex_destroy(&(*signal)->mutex);
		pthread_cond_destroy(&(*signal)->cond);
		free(*signal);
		*signal	 = NULL;
		ret_code = 0;
	}
	return ret_code;
}