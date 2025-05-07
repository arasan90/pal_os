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
typedef void (*pal_timer_callback_t)(void *arg);

// ============================
// Function Declarations
// ============================
int pal_timer_create(pal_timer_t **timer, pal_timer_type_t type, size_t period, pal_timer_callback_t callback, int auto_start, void *arg);

int pal_timer_start(pal_timer_t *timer);

int pal_timer_stop(pal_timer_t *timer);

int pal_timer_restart(pal_timer_t *timer);

int pal_timer_change_period(pal_timer_t *timer, size_t new_period);

int pal_is_timer_active(pal_timer_t *timer);

int pal_timer_delete(pal_timer_t **timer);

#ifdef __cplusplus
}
#endif
