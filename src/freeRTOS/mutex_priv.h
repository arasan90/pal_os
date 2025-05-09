#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

// ============================
// Includes
// ============================
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "pal_os/mutex.h"

// ============================
// Macros and Constants
// ============================

// ============================
// Type Definitions
// ============================
struct pal_mutex_s
{
	SemaphoreHandle_t mutex_handle;	 //!< Mutex handle
	int				  is_recursive;	 //!< Flag indicating if the mutex is recursive
};

// ============================
// Function Declarations
// ============================

#ifdef __cplusplus
}
#endif
