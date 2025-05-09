#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

// ============================
// Includes
// ============================
#include <stddef.h>

// ============================
// Macros and Constants
// ============================

// ============================
// Type Definitions
// ============================
/**
 * @brief Timer type
 */
typedef enum pal_timer_type_e
{
	PAL_TIMER_TYPE_ONESHOT,	  //!< One-shot timer
	PAL_TIMER_TYPE_PERIODIC,  //!< Periodic timer
} pal_timer_type_t;
/**
 * @brief Timer handle
 */
typedef struct pal_timer_s pal_timer_t;

/**
 * @brief Timer callback function type
 *
 * @param arg Pointer to user-defined argument passed to the callback function
 */
typedef void (*pal_timer_callback_t)(pal_timer_t *arg);

// ============================
// Function Declarations
// ============================
/**
 * @brief Creates a new timer.
 *
 * @param[out] timer Pointer to the timer handle to be created.
 * @param[in] name Name of the timer. This is used for debugging purposes.
 * @note Not available in linux.
 * @param[in] type Type of the timer (one-shot or periodic).
 * @param[in] period Timer period in milliseconds. Cannot be 0.
 * @param[in] callback Function to be called when the timer expires.
 * @param[in] auto_start If non-zero, the timer starts automatically after creation.
 * @param[in] arg User-defined argument passed to the callback function.
 * @return 0 on success, or -1 on failure.
 */
int pal_timer_create(pal_timer_t **timer, const char *name, pal_timer_type_t type, size_t period, pal_timer_callback_t callback, int auto_start, void *arg);

/**
 * @brief Starts a stopped timer.
 *
 * @param[in] timer Pointer to the timer handle.
 * @param[in] from_isr If non-zero, the function is called from an ISR context.
 * @return 0 on success, or -1 on failure.
 */
int pal_timer_start(pal_timer_t *timer, int from_isr);

/**
 * @brief Stops a running timer.
 *
 * @param[in] timer Pointer to the timer handle.
 * @param[in] from_isr If non-zero, the function is called from an ISR context.
 * @return 0 on success, or -1 on failure.
 */
int pal_timer_stop(pal_timer_t *timer, int from_isr);

/**
 * @brief Restarts a timer.
 *
 * @param[in] timer Pointer to the timer handle.
 * @param[in] from_isr If non-zero, the function is called from an ISR context.
 * @return 0 on success, or -1 on failure.
 */
int pal_timer_restart(pal_timer_t *timer, int from_isr);

/**
 * @brief Changes the period of a timer.
 *
 * @param[in] timer Pointer to the timer handle.
 * @param[in] new_period New timer period in milliseconds. Cannot be 0.
 * @param[in] from_isr If non-zero, the function is called from an ISR context.
 * @return 0 on success, or -1 on failure.
 */
int pal_timer_change_period(pal_timer_t *timer, size_t new_period, int from_isr);

/**
 * @brief Checks if a timer is active.
 *
 * @param[in] timer Pointer to the timer handle.
 * @return 1 if the timer is active, 0 if it is not active or timer does not exist.
 */
int pal_is_timer_active(pal_timer_t *timer);

/**
 * @brief Deletes a timer.
 *
 * @param[in,out] timer Pointer to the timer handle to be deleted. The pointer is set to NULL after deletion.
 * @return 0 on success, or -1 on failure.
 */
int pal_timer_delete(pal_timer_t **timer);

#ifdef __cplusplus
}
#endif
