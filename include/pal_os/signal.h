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
#include "pal_os/common.h"

// ============================
// Macros and Constants
// ============================

// ============================
// Type Definitions
// ============================

#ifdef PAL_OS_LINUX
struct pal_signal_s
{
	pthread_mutex_t mutex;	  //!< Mutex for thread safety.
	pthread_cond_t	cond;	  //!< Condition variable for signaling.
	size_t			signals;  //!< Bitmask of active signals.
};
typedef struct pal_signal_s pal_signal_t;

#elif defined PAL_OS_FREERTOS
typedef void *pal_signal_t;

#endif

/**
 * @brief Return codes for signal wait operation.
 */
typedef enum pal_signal_ret_code_e
{
	PAL_SIGNAL_TIMEOUT = -2,  //!< Operation timed out
	PAL_SIGNAL_FAILURE = -1,  //!< Operation failed
	PAL_SIGNAL_SUCCESS = 0,	  //!< Operation succeeded
} pal_signal_ret_code_t;

// ============================
// Function Declarations
// ============================

/**
 * @brief Create a signal object.
 *
 * @param[out] signal Pointer to the signal object to initialize.
 * @return 0 on success, or -1 on failure.
 */
int pal_signal_create(pal_signal_t *signal);

/**
 * @brief Waits for one or more signals to be set.
 *
 * @param[in] signal Signal object to wait on.
 * @param[in] mask Bitmask of signals to wait for.
 * @param[out] received_signals Pointer to store the bitmask of received signals.
 * @param[in] clear_mask If 1, clear received signals before returning; if 0, do not clear the signals.
 * @param[in] wait_all If 1, wait for all specified signals; if 0, wait for any.
 * @param[in] timeout_ms Timeout in milliseconds. Use PAL_OS_NO_TIMEOUT for non-blocking or PAL_OS_INFINITE_TIMEOUT for infinite wait.
 * @return PAL_SIGNAL_SUCCESS if signals are received within the timeout, PAL_SIGNAL_TIMEOUT if timeout occurs, or PAL_SIGNAL_FAILURE on error.
 * @note This function blocks until the specified signals are set or the timeout occurs.
 */
pal_signal_ret_code_t pal_signal_wait(pal_signal_t *signal, size_t mask, size_t *received_signals, int clear_mask, int wait_all, size_t timeout_ms);

/**
 * @brief Sets one or more signals.
 *
 * @param[in] signal Signal object to modify.
 * @param[in] mask Bitmask of signals to set.
 * @return 0 on success, or -1 on failure.
 */
int pal_signal_set(pal_signal_t *signal, size_t mask);

/**
 * @brief Sets one or more signals from ISR.
 *
 * @param[in] signal Signal object to modify.
 * @param[in] mask Bitmask of signals to set.
 * @return 0 on success, or -1 on failure.
 */
int pal_signal_set(pal_signal_t *signal, size_t mask);

/**
 * @brief Clears one or more signals.
 *
 * @param[in] signal Signal object to modify.
 * @param[in] mask Bitmask of signals to clear.
 * @return 0 on success, or -1 on failure.
 */
int pal_signal_clear(pal_signal_t *signal, size_t mask);

/**
 * @brief Destroys a signal object and releases associated resources.
 *
 * @param[in,out] signal Pointer to the signal object to destroy.
 * @return 0 on success, or -1 on failure.
 */
int pal_signal_destroy(pal_signal_t *signal);

#ifdef __cplusplus
}
#endif