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
#elif defined PAL_OS_FREERTOS
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#endif

// ============================
// Macros and Constants
// ============================

// ============================
// Type Definitions
// ============================
#ifdef PAL_OS_LINUX
struct pal_mutex_s
{
	pthread_mutex_t mutex;	//!< Mutex for thread safety
};
#elif defined PAL_OS_FREERTOS
struct pal_mutex_s
{
	SemaphoreHandle_t mutex_handle;	 //!< Mutex handle
	int				  is_recursive;	 //!< Flag indicating if the mutex is recursive
};
#endif
typedef struct pal_mutex_s pal_mutex_t;

// ============================
// Function Declarations
// ============================

/**
 * @brief Creates a new mutex.
 *
 * @param[out] mutex Pointer to the mutex handle to be created.
 * @param[in] recursive Indicates whether the mutex should be recursive (non-zero for recursive).
 * @return 0 on success, or -1 on failure.
 */
int pal_mutex_create(pal_mutex_t *mutex, int recursive);

/**
 * @brief Locks the specified mutex.
 *
 * @param[in] mutex Pointer to the mutex to lock.
 * @param[in] timeout_ms Timeout in milliseconds to wait for the lock. Use PAL_OS_NO_TIMEOUT for no wait, or PAL_OS_INFINITE_TIMEOUT for infinite wait.
 * @return 0 on success, or -1 on failure (e.g., timeout).
 */
int pal_mutex_lock(pal_mutex_t *mutex, size_t timeout_ms);

/**
 * @brief Unlocks the specified mutex.
 *
 * @param[in] mutex Pointer to the mutex to unlock.
 * @return 0 on success, or -1 on failure.
 */
int pal_mutex_unlock(pal_mutex_t *mutex);

/**
 * @brief Destroys the specified mutex.
 *
 * @param[in,out] mutex Pointer to the mutex handle to be destroyed.
 * @return 0 on success, or -1 on failure.
 */
int pal_mutex_destroy(pal_mutex_t *mutex);

#ifdef __cplusplus
}
#endif
