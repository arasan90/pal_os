/*
 * File: queue.c
 * Description: Implementation of queue-related functionality for the Linux platform.
 * Author: Massimiliano Ianniello
 */

#include "pal_os/queue.h"

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
int pal_queue_create(pal_queue_t *queue, size_t item_size, size_t max_items)
{
	int ret_code = -1;
	if (NULL != queue && 0 != item_size && 0 != max_items)
	{
		queue->data = malloc(item_size * max_items);
		if (NULL != queue->data)
		{
			queue->item_size = item_size;
			queue->max_items = max_items;
			queue->head		 = 0;
			queue->tail		 = 0;
			pthread_mutexattr_t attr;
			pthread_mutexattr_init(&attr);
			pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
			pthread_mutex_init(&queue->mutex, &attr);
			pthread_cond_init(&queue->full, NULL);
			pthread_cond_init(&queue->empty, NULL);
			ret_code = 0;
		}
	}
	return ret_code;
}

int pal_queue_enqueue(pal_queue_t *queue, void *const item, size_t timeout_ms)
{
	int ret_code = -1;
	int error	 = 0;
	if (NULL != queue && NULL != item)
	{
		pthread_mutex_lock(&queue->mutex);
		size_t free_slots = queue->max_items - (queue->tail - queue->head);
		if (0 == free_slots)
		{
			if (PAL_OS_NO_TIMEOUT == timeout_ms)
			{
				error = 1;
			}
			else if (PAL_OS_INFINITE_TIMEOUT == timeout_ms)
			{
				pthread_cond_wait(&queue->full, &queue->mutex);
			}
			else
			{
				struct timespec timeout = {0};
				clock_gettime(CLOCK_REALTIME, &timeout);
				timeout.tv_sec += timeout_ms / 1000;
				timeout.tv_nsec += (timeout_ms % 1000) * 1000000;
				timeout.tv_sec += timeout.tv_nsec / 1000000000;
				timeout.tv_nsec = timeout.tv_nsec % 1000000000;
				if (ETIMEDOUT == pthread_cond_timedwait(&queue->full, &queue->mutex, &timeout))
				{
					error = 1;
				}
			}
		}
		if (!error)
		{
			int idx = queue->tail % queue->max_items;
			memcpy((char *)queue->data + (idx * queue->item_size), item, queue->item_size);
			queue->tail++;
			ret_code = 0;
		}
		pthread_cond_signal(&queue->empty);
		pthread_mutex_unlock(&queue->mutex);
	}
	return ret_code;
}

int pal_queue_enqueue_from_isr(pal_queue_t *queue, void *const item, size_t timeout_ms)
{
	int ret_code = -1;
	int error	 = 0;
	if (NULL != queue && NULL != item)
	{
		pthread_mutex_lock(&queue->mutex);
		size_t free_slots = queue->max_items - (queue->tail - queue->head);
		if (0 == free_slots)
		{
			if (PAL_OS_NO_TIMEOUT == timeout_ms)
			{
				error = 1;
			}
			else if (PAL_OS_INFINITE_TIMEOUT == timeout_ms)
			{
				pthread_cond_wait(&queue->full, &queue->mutex);
			}
			else
			{
				struct timespec timeout = {0};
				clock_gettime(CLOCK_REALTIME, &timeout);
				timeout.tv_sec += timeout_ms / 1000;
				timeout.tv_nsec += (timeout_ms % 1000) * 1000000;
				timeout.tv_sec += timeout.tv_nsec / 1000000000;
				timeout.tv_nsec = timeout.tv_nsec % 1000000000;
				if (ETIMEDOUT == pthread_cond_timedwait(&queue->full, &queue->mutex, &timeout))
				{
					error = 1;
				}
			}
		}
		if (!error)
		{
			int idx = queue->tail % queue->max_items;
			memcpy((char *)queue->data + (idx * queue->item_size), item, queue->item_size);
			queue->tail++;
			ret_code = 0;
		}
		pthread_cond_signal(&queue->empty);
		pthread_mutex_unlock(&queue->mutex);
	}
	return ret_code;
}

int pal_queue_dequeue(pal_queue_t *queue, void *const item, size_t timeout_ms)
{
	int ret_code = -1;
	int error	 = 0;
	if (NULL != queue && NULL != item)
	{
		pthread_mutex_lock(&queue->mutex);
		size_t used_slots = queue->tail - queue->head;
		if (0 == used_slots)
		{
			if (PAL_OS_NO_TIMEOUT == timeout_ms)
			{
				error = 1;
			}
			else if (PAL_OS_INFINITE_TIMEOUT == timeout_ms)
			{
				pthread_cond_wait(&queue->empty, &queue->mutex);
			}
			else
			{
				struct timespec timeout = {0};
				clock_gettime(CLOCK_REALTIME, &timeout);
				timeout.tv_sec += timeout_ms / 1000;
				timeout.tv_nsec += (timeout_ms % 1000) * 1000000;
				timeout.tv_sec += timeout.tv_nsec / 1000000000;
				timeout.tv_nsec = timeout.tv_nsec % 1000000000;
				if (ETIMEDOUT == pthread_cond_timedwait(&queue->empty, &queue->mutex, &timeout))
				{
					error = 1;
				}
			}
		}
		if (!error)
		{
			int idx = queue->head % queue->max_items;
			memcpy(item, (char *)queue->data + (idx * queue->item_size), queue->item_size);
			queue->head++;
			ret_code = 0;
		}
		pthread_cond_signal(&queue->full);
		pthread_mutex_unlock(&queue->mutex);
	}
	return ret_code;
}

int pal_queue_dequeue_from_isr(pal_queue_t *queue, void *const item, size_t timeout_ms)
{
	int ret_code = -1;
	int error	 = 0;
	if (NULL != queue && NULL != item)
	{
		pthread_mutex_lock(&queue->mutex);
		size_t used_slots = queue->tail - queue->head;
		if (0 == used_slots)
		{
			if (PAL_OS_NO_TIMEOUT == timeout_ms)
			{
				error = 1;
			}
			else if (PAL_OS_INFINITE_TIMEOUT == timeout_ms)
			{
				pthread_cond_wait(&queue->empty, &queue->mutex);
			}
			else
			{
				struct timespec timeout = {0};
				clock_gettime(CLOCK_REALTIME, &timeout);
				timeout.tv_sec += timeout_ms / 1000;
				timeout.tv_nsec += (timeout_ms % 1000) * 1000000;
				timeout.tv_sec += timeout.tv_nsec / 1000000000;
				timeout.tv_nsec = timeout.tv_nsec % 1000000000;
				if (ETIMEDOUT == pthread_cond_timedwait(&queue->empty, &queue->mutex, &timeout))
				{
					error = 1;
				}
			}
		}
		if (!error)
		{
			int idx = queue->head % queue->max_items;
			memcpy(item, (char *)queue->data + (idx * queue->item_size), queue->item_size);
			queue->head++;
			ret_code = 0;
		}
		pthread_cond_signal(&queue->full);
		pthread_mutex_unlock(&queue->mutex);
	}
	return ret_code;
}

void pal_queue_reset(pal_queue_t *queue)
{
	pthread_mutex_lock(&queue->mutex);
	queue->tail = queue->head = 0;
	pthread_mutex_unlock(&queue->mutex);
}

size_t pal_queue_get_free_slots(pal_queue_t *queue)
{
	size_t free_slots = 0;
	if (NULL != queue)
	{
		pthread_mutex_lock(&queue->mutex);
		free_slots = queue->max_items - (queue->tail - queue->head);
		pthread_mutex_unlock(&queue->mutex);
	}
	return free_slots;
}

size_t pal_queue_get_items(pal_queue_t *queue)
{
	size_t items = 0;
	if (NULL != queue)
	{
		pthread_mutex_lock(&queue->mutex);
		items = queue->tail - queue->head;
		pthread_mutex_unlock(&queue->mutex);
	}
	return items;
}

size_t pal_queue_get_items_from_isr(pal_queue_t *queue)
{
	size_t items = 0;
	if (NULL != queue)
	{
		pthread_mutex_lock(&queue->mutex);
		items = queue->tail - queue->head;
		pthread_mutex_unlock(&queue->mutex);
	}
	return items;
}

void pal_queue_destroy(pal_queue_t *queue)
{
	if (NULL != queue)
	{
		pthread_mutex_destroy(&queue->mutex);
		pthread_cond_destroy(&queue->full);
		pthread_cond_destroy(&queue->empty);
		free(queue->data);
	}
}