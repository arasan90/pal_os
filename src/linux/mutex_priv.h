#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

// ============================
// Includes
// ============================
#include <pthread.h>

#include "pal_os/mutex.h"
// ============================
// Macros and Constants
// ============================

// ============================
// Type Definitions
// ============================
struct pal_mutex_s
{
	pthread_mutex_t mutex;	//!< Mutex for thread safety
};

// ============================
// Function Declarations
// ============================

#ifdef __cplusplus
}
#endif
