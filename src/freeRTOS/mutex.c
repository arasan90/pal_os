/*
 * File: mutex->c
 * Description: Implementation of mutex-related functionality for the Linux platform.
 * Author: Massimiliano Ianniello
 */

#include "pal_os/mutex.h"

#include <string.h>

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
		if (recursive)
		{
			mutex->is_recursive = 1;
			mutex->mutex_handle = xSemaphoreCreateRecursiveMutex();
		}
		else
		{
			mutex->mutex_handle = xSemaphoreCreateMutex();
		}
		if (mutex->mutex_handle)
		{
			ret_code = 0;
		}
	}
	return ret_code;
}

int pal_mutex_lock(pal_mutex_t *mutex, size_t timeout_ms)
{
	int ret_code = -1;
	if (mutex)
	{
		TickType_t timeout_ticks = portMAX_DELAY;
		if (PAL_OS_INFINITE_TIMEOUT != timeout_ms)
		{
			timeout_ticks = pdMS_TO_TICKS(timeout_ms);
		}
		if (mutex->is_recursive)
		{
			ret_code = pdTRUE == xSemaphoreTakeRecursive(mutex->mutex_handle, timeout_ticks) ? 0 : -1;
		}
		else
		{
			ret_code = pdTRUE == xSemaphoreTake(mutex->mutex_handle, timeout_ticks) ? 0 : -1;
		}
	}
	return ret_code;
}

int pal_mutex_unlock(pal_mutex_t *mutex)
{
	int ret_code = -1;
	if (mutex)
	{
		if (mutex->is_recursive)
		{
			ret_code = pdTRUE == xSemaphoreGiveRecursive(mutex->mutex_handle) ? 0 : -1;
		}
		else
		{
			ret_code = pdTRUE == xSemaphoreGive(mutex->mutex_handle) ? 0 : -1;
		}
	}
	return ret_code;
}

int pal_mutex_destroy(pal_mutex_t *mutex)
{
	int ret_code = -1;
	if (mutex && mutex->mutex_handle)
	{
		vSemaphoreDelete(mutex->mutex_handle);
		ret_code = 0;
	}
	return ret_code;
}