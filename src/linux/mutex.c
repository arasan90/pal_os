/*
 * File: mutex.c
 * Description: Implementation of mutex-related functionality for the Linux platform.
 * Author: Massimiliano Ianniello
 */

#include "pal_os/mutex.h"

#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <time.h>

#include "pal_os/common.h"
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
int pal_mutex_create(pal_mutex_t *mutex, int recursive)
{
	int ret_code = -1;
	if (mutex)
	{
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		if (recursive)
		{
			pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		}
		pthread_mutex_init(&mutex->mutex, &attr);
		pthread_mutexattr_destroy(&attr);
		ret_code = 0;
	}
	return ret_code;
}

int pal_mutex_lock(pal_mutex_t *mutex, size_t timeout_ms)
{
	int ret_code = -1;
	if (mutex)
	{
		switch (timeout_ms)
		{
			case PAL_OS_NO_TIMEOUT:
				ret_code = 0 == pthread_mutex_trylock(&mutex->mutex) ? 0 : -1;
				break;
			case PAL_OS_INFINITE_TIMEOUT:
				ret_code = 0 == pthread_mutex_lock(&mutex->mutex) ? 0 : -1;
				break;
			default:
			{
				struct timespec timeout = {0};
				clock_gettime(CLOCK_REALTIME, &timeout);
				timeout.tv_sec += timeout_ms / 1000;
				timeout.tv_nsec += (timeout_ms % 1000) * 1000000;
				timeout.tv_sec += timeout.tv_nsec / 1000000000;
				timeout.tv_nsec = timeout.tv_nsec % 1000000000;
				ret_code		= 0 == pthread_mutex_timedlock(&mutex->mutex, &timeout) ? 0 : -1;
			}
			break;
		}
	}
	return ret_code;
}

int pal_mutex_unlock(pal_mutex_t *mutex)
{
	int ret_code = -1;
	if (mutex)
	{
		ret_code = 0 == pthread_mutex_unlock(&mutex->mutex) ? 0 : -1;
	}
	return ret_code;
}

int pal_mutex_destroy(pal_mutex_t *mutex)
{
	int ret_code = -1;
	if (mutex)
	{
		pthread_mutex_destroy(&mutex->mutex);
		ret_code = 0;
	}
	return ret_code;
}