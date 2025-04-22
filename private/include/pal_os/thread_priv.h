#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

// ============================
// Includes
// ============================
#include <pthread.h>
#include <stddef.h>

#include "pal_os/thread.h"

// ============================
// Macros and Constants
// ============================

// ============================
// Type Definitions
// ============================
struct pal_thread_s
{
	pthread_t			  thread;	   // POSIX thread identifier
	pal_thread_func_t	  func;		   // Function to be executed by the thread
	void				 *arg;		   // Argument to be passed to the thread function
	pal_thread_state_t	  state;	   // State of the thread
	size_t				  stack_size;  // Size of the thread stack
	pal_thread_priority_t priority;	   // Thread priority
	const char			 *name;		   // Thread name
};

// ============================
// Function Declarations
// ============================
/**
 * @brief Thread function wrapper.
 * @param arg Pointer to the thread structure.
 * @return NULL.
 */
void *pal_thread_generic_func(void *const arg);

#ifdef __cplusplus
}
#endif
