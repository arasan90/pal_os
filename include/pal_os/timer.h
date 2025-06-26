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
#include <time.h>

#endif
// ============================
// Macros and Constants
// ============================

// ============================
// Type Definitions
// ============================

#ifdef PAL_OS_LINUX
struct pal_timer_s
{
	struct timespec expiry_time;			 //!< Expiry time of the timer
	void (*callback)(struct pal_timer_s *);	 //!< Callback function to be called when the timer expires
	void			   *arg;				 //!< User-defined argument passed to the callback function
	int					is_periodic;		 //!< Flag indicating if the timer is periodic
	int					is_started;			 //!< Flag indicating if the timer is started
	size_t				period_ms;			 //!< Timer period in milliseconds
	struct pal_timer_s *next;				 //!< Pointer to the next timer in the list
};
typedef struct pal_timer_s pal_timer_t;

#elif PAL_OS_FREERTOS
typedef void *pal_timer_t;

#endif
/**
 * @brief Timer type
 */
typedef enum pal_timer_type_e
{
	PAL_TIMER_TYPE_ONESHOT,	  //!< One-shot timer
	PAL_TIMER_TYPE_PERIODIC,  //!< Periodic timer
} pal_timer_type_t;

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
int pal_timer_create(pal_timer_t *timer, const char *name, pal_timer_type_t type, size_t period, pal_timer_callback_t callback, int auto_start, void *arg);

/**
 * @brief Starts a stopped timer.
 *
 * @param[in] timer Pointer to the timer handle.
 * @return 0 on success, or -1 on failure.
 */
int pal_timer_start(pal_timer_t *timer);

/**
 * @brief Starts a stopped timer from ISR.
 *
 * @param[in] timer Pointer to the timer handle.
 * @return 0 on success, or -1 on failure.
 */
int pal_timer_start_from_isr(pal_timer_t *timer);

/**
 * @brief Stops a running timer.
 *
 * @param[in] timer Pointer to the timer handle.
 * @return 0 on success, or -1 on failure.
 */
int pal_timer_stop(pal_timer_t *timer);

/**
 * @brief Stops a running timer from ISR.
 *
 * @param[in] timer Pointer to the timer handle.
 * @return 0 on success, or -1 on failure.
 */
int pal_timer_stop_from_isr(pal_timer_t *timer);

/**
 * @brief Restarts a timer.
 *
 * @param[in] timer Pointer to the timer handle.
 * @return 0 on success, or -1 on failure.
 */
int pal_timer_restart(pal_timer_t *timer);

/**
 * @brief Restarts a timer from ISR.
 *
 * @param[in] timer Pointer to the timer handle.
 * @return 0 on success, or -1 on failure.
 */
int pal_timer_restart_from_isr(pal_timer_t *timer);

/**
 * @brief Changes the period of a timer.
 *
 * @param[in] timer Pointer to the timer handle.
 * @param[in] new_period New timer period in milliseconds. Cannot be 0.
 * @return 0 on success, or -1 on failure.
 */
int pal_timer_change_period(pal_timer_t *timer, size_t new_period);

/**
 * @brief Changes the period of a timer from ISR.
 *
 * @param[in] timer Pointer to the timer handle.
 * @param[in] new_period New timer period in milliseconds. Cannot be 0.
 * @return 0 on success, or -1 on failure.
 */
int pal_timer_change_period_from_isr(pal_timer_t *timer, size_t new_period);

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
 * @param[in,out] timer Pointer to the timer handle to be deleted.
 * @return 0 on success, or -1 on failure.
 */
int pal_timer_delete(pal_timer_t *timer);

#ifdef __cplusplus
}
#endif
