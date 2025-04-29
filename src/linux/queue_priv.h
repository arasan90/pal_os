#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

// ============================
// Includes
// ============================
#include <pthread.h>
#include <semaphore.h>

#include "pal_os/queue.h"
// ============================
// Macros and Constants
// ============================

// ============================
// Type Definitions
// ============================
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

// ============================
// Function Declarations
// ============================

#ifdef __cplusplus
}
#endif
