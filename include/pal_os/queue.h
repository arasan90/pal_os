#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

// ============================
// Includes
// ============================
#include <stddef.h>
#ifdef PAL_OS_LINUX
#include <pthread.h>
#include <semaphore.h>
#endif

// ============================
// Macros and Constants
// ============================

// ============================
// Type Definitions
// ============================
#ifdef PAL_OS_LINUX
struct pal_queue_s
{
	size_t			item_size;	//!< Size of each item in the queue
	size_t			max_items;	//!< Maximum number of items in the queue
	size_t			head;		//!< Index of the head of the queue
	size_t			tail;		//!< Index of the tail of the queue
	pthread_mutex_t mutex;		//!< Mutex for thread safety
	pthread_cond_t	full;		//!< Condition variable for full queue
	pthread_cond_t	empty;		//!< Condition variable for empty queue
	void		   *data;		//!< Pointer to the queue data
};
typedef struct pal_queue_s pal_queue_t;

#elif defined PAL_OS_FREERTOS
typedef void *pal_queue_t;

#endif

// ============================
// Function Declarations
// ============================

/**
 * @brief Create a queue with specified item size and maximum number of items.
 *
 * @param[out] queue Pointer to the queue handle to be created.
 * @param[in] item_size Size of each item in the queue.
 * @param[in] max_items Maximum number of items the queue can hold.
 * @return 0 on success, or -1 on failure.
 */
int pal_queue_create(pal_queue_t *queue, size_t item_size, size_t max_items);

/**
 * @brief Enqueue an item into the queue.
 *
 * @param[in] queue Pointer to the queue handle.
 * @param[in] item Pointer to the item to be enqueued.
 * @param[in] timeout_ms Timeout in milliseconds to wait if the queue is full.
 * @return 0 on success, or -1 on failure.
 */
int pal_queue_enqueue(pal_queue_t *queue, void *const item, size_t timeout_ms);

/**
 * @brief Enqueue an item into the queue from ISR.
 *
 * @param[in] queue Pointer to the queue handle.
 * @param[in] item Pointer to the item to be enqueued.
 * @param[in] timeout_ms Timeout in milliseconds to wait if the queue is full.
 * @return 0 on success, or -1 on failure.
 */
int pal_queue_enqueue_from_isr(pal_queue_t *queue, void *const item, size_t timeout_ms);

/**
 * @brief Dequeue an item from the queue.
 *
 * @param[in] queue Pointer to the queue handle.
 * @param[out] item Pointer to the memory where the dequeued item will be stored.
 * @param[in] timeout_ms Timeout in milliseconds to wait if the queue is empty.
 * @return 0 on success, or -1 on failure.
 */
int pal_queue_dequeue(pal_queue_t *queue, void *const item, size_t timeout_ms);

/**
 * @brief Dequeue an item from the queue from ISR.
 *
 * @param[in] queue Pointer to the queue handle.
 * @param[out] item Pointer to the memory where the dequeued item will be stored.
 * @param[in] timeout_ms Timeout in milliseconds to wait if the queue is empty.
 * @return 0 on success, or -1 on failure.
 */
int pal_queue_dequeue_from_isr(pal_queue_t *queue, void *const item, size_t timeout_ms);

/**
 * @brief Reset the queue.
 *
 * @param[in] queue Pointer to the queue handle.
 */
void pal_queue_reset(pal_queue_t *queue);

/**
 * @brief Get the number of free slots available in the queue.
 *
 * @param[in] queue Pointer to the queue handle.
 * @return Number of free slots in the queue.
 * @note Will return 0 if the queue is NULL.
 */
size_t pal_queue_get_free_slots(pal_queue_t *queue);

/**
 * @brief Get the number of items currently in the queue.
 *
 * @param[in] queue Pointer to the queue handle.
 * @return Number of items in the queue.
 * @note Will return 0 if the queue is NULL.
 */
size_t pal_queue_get_items(pal_queue_t *queue);

/**
 * @brief Get the number of items currently in the queue from ISR.
 *
 * @param[in] queue Pointer to the queue handle.
 * @return Number of items in the queue.
 * @note Will return 0 if the queue is NULL.
 */
size_t pal_queue_get_items_from_isr(pal_queue_t *queue);

/**
 * @brief Destroy the queue and releases associated resources.
 *
 * @param[in,out] queue Pointer to the queue handle to be destroyed.
 */
void pal_queue_destroy(pal_queue_t *queue);

#ifdef __cplusplus
}
#endif
