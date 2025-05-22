/*
 * File: queue.c
 * Description: Implementation of queue-related functionality for the freeRTOS platform.
 * Author: Massimiliano Ianniello
 */

#include "pal_os/queue.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
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
int pal_queue_create(pal_queue_t *queue, size_t item_size, size_t max_items)
{
	int ret_code = -1;
	if (queue && item_size && max_items)
	{
		*queue	 = (pal_queue_t)xQueueCreate(max_items, item_size);
		ret_code = *queue ? 0 : -1;
	}
	return ret_code;
}

int pal_queue_enqueue(pal_queue_t *queue, void *const item, size_t timeout_ms, int from_isr)
{
	int ret_code = -1;
	if (queue && item)
	{
		TickType_t timeout_ticks = portMAX_DELAY;
		if (timeout_ms != PAL_OS_INFINITE_TIMEOUT)
		{
			// Convert timeout_ms to ticks
			timeout_ticks = pdMS_TO_TICKS(timeout_ms);
		}
		if (!from_isr)
		{
			ret_code = pdTRUE == xQueueSend((QueueHandle_t)*queue, item, timeout_ms) ? 0 : -1;
		}
		else
		{
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			ret_code							= pdTRUE == xQueueSendFromISR((QueueHandle_t)*queue, item, &xHigherPriorityTaskWoken) ? 0 : -1;
			if (xHigherPriorityTaskWoken)
			{
				portYIELD_FROM_ISR();
			}
		}
	}
	return ret_code;
}

int pal_queue_dequeue(pal_queue_t *queue, void *const item, size_t timeout_ms, int from_isr)
{
	int ret_code = -1;
	if (queue && item)
	{
		TickType_t timeout_ticks = portMAX_DELAY;
		if (timeout_ms != PAL_OS_INFINITE_TIMEOUT)
		{
			// Convert timeout_ms to ticks
			timeout_ticks = pdMS_TO_TICKS(timeout_ms);
		}
		if (!from_isr)
		{
			ret_code = pdTRUE == xQueueReceive((QueueHandle_t)*queue, item, timeout_ms) ? 0 : -1;
		}
		else
		{
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			ret_code							= pdTRUE == xQueueReceiveFromISR((QueueHandle_t)*queue, item, &xHigherPriorityTaskWoken) ? 0 : -1;
			if (xHigherPriorityTaskWoken)
			{
				portYIELD_FROM_ISR();
			}
		}
	}
	return ret_code;
}

void pal_queue_reset(pal_queue_t *queue)
{
	if (queue)
	{
		xQueueReset((QueueHandle_t)*queue);
	}
}

size_t pal_queue_get_free_slots(pal_queue_t *queue)
{
	size_t free_slots = 0;
	if (queue)
	{
		free_slots = uxQueueSpacesAvailable((QueueHandle_t)*queue);
	}
	return free_slots;
}

size_t pal_queue_get_items(pal_queue_t *queue, int from_isr)
{
	size_t items = 0;
	if (queue)
	{
		if (!from_isr)
		{
			items = uxQueueMessagesWaiting((QueueHandle_t)*queue);
		}
		else
		{
			items = uxQueueMessagesWaitingFromISR((QueueHandle_t)*queue);
		}
	}
	return items;
}

void pal_queue_destroy(pal_queue_t *queue)
{
	if (queue && *queue)
	{
		vQueueDelete((QueueHandle_t)*queue);
		*queue = NULL;
	}
}