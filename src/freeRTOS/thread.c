/*
 * File: thread.c
 * Description: Implementation of thread-related functionality for the freeRTOS platform.
 * Author: Massimiliano Ianniello
 */

#include "pal_os/thread.h"

#include <string.h>

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
#define PAL_THREAD_THREAD_EXITED_EVENT_BITS (1 << 0)  //!< Event bit for thread exit

/* ---------------------------------------------------------------------------
 * Static Functions
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Function Implementations
 * ---------------------------------------------------------------------------
 */
UBaseType_t pal_thread_convert_priority(pal_thread_priority_t pal_priority)
{
	UBaseType_t freertos_priority = 1;
	switch (pal_priority)
	{
		case PAL_THREAD_PRIORITY_LOW:
			freertos_priority = configMAX_PRIORITIES / 3;
			break;
		case PAL_THREAD_PRIORITY_NORMAL:
			freertos_priority = configMAX_PRIORITIES / 2;
			break;
		case PAL_THREAD_PRIORITY_HIGH:
			freertos_priority = 2 * configMAX_PRIORITIES / 3;
			break;
		default:
			break;
	}
	return freertos_priority;
}

void pal_thread_generic_func(void *const arg)
{
	pal_thread_t *thread = (pal_thread_t *)arg;
	if (thread->func)
	{
		thread->state = PAL_THREAD_STATE_RUNNING;
		thread->func(thread->arg);
	}
	xEventGroupSetBits(thread->event_group_handle, PAL_THREAD_THREAD_EXITED_EVENT_BITS);
	vTaskDelete(NULL);
}

int pal_thread_create(pal_thread_t *thread, pal_thread_priority_t priority, size_t stack_size, pal_thread_func_t func, const char *name, void *arg)
{
	int ret_code = -1;
	if (thread && NULL != func && 0 != stack_size)
	{
		thread->event_group_handle = xEventGroupCreate();
		if (thread->event_group_handle)
		{
			UBaseType_t freertos_priority = pal_thread_convert_priority(priority);
			thread->func				  = func;
			thread->arg					  = arg;
			thread->state				  = PAL_THREAD_STATE_STOPPED;
			if (pdPASS ==
				xTaskCreate(pal_thread_generic_func, name, stack_size / sizeof(StackType_t), arg, freertos_priority, (TaskHandle_t *)thread->thread_handle))
			{
				ret_code = 0;
			}
			else
			{
				vEventGroupDelete(thread->event_group_handle);
			}
		}
	}
	return ret_code;
}

void pal_thread_sleep(size_t time_ms) { vTaskDelay(pdMS_TO_TICKS(time_ms)); }

void pal_thread_scheduler_start(void) { vTaskStartScheduler(); }

void pal_thread_join(pal_thread_t *const thread)
{
	xEventGroupWaitBits(thread->event_group_handle, PAL_THREAD_THREAD_EXITED_EVENT_BITS, pdTRUE, pdTRUE, portMAX_DELAY);
	thread->state = PAL_THREAD_STATE_TERMINATED;
}

const char *pal_thread_get_name(pal_thread_t *const thread)
{
	char *thread_name = NULL;
	if (thread && thread->thread_handle)
	{
		thread_name = pcTaskGetName(thread->thread_handle);
	}
	return thread_name;
}

size_t pal_thread_get_stack_watermark(pal_thread_t *const thread)
{
	size_t stack_watermark = 0;
	if (thread && thread->thread_handle)
	{
#if INCLUDE_uxTaskGetStackHighWaterMark
		stack_watermark = uxTaskGetStackHighWaterMark(thread->thread_handle);
#endif
	}
	return stack_watermark;
}

void pal_thread_free(pal_thread_t *thread)
{
	if (thread && PAL_THREAD_STATE_TERMINATED == thread->state)
	{
		vEventGroupDelete(thread->event_group_handle);
		thread->event_group_handle = NULL;
	}
}