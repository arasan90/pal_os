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

#include "pal_os/signal.h"
// ============================
// Macros and Constants
// ============================

// ============================
// Type Definitions
// ============================
/**
 * @brief Internal structure representing a signal object.
 */
struct pal_signal_s
{
	pthread_mutex_t mutex;	  //!< Mutex for thread safety.
	pthread_cond_t	cond;	  //!< Condition variable for signaling.
	size_t			signals;  //!< Bitmask of active signals.
};

// ============================
// Function Declarations
// ============================

#ifdef __cplusplus
}
#endif
