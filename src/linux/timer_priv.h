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
#include <time.h>

#include "pal_os/timer.h"
// ============================
// Macros and Constants
// ============================

// ============================
// Type Definitions
// ============================
struct pal_timer_s
{
	struct timespec		 expiry_time;  //!< Expiry time of the timer
	pal_timer_callback_t callback;	   //!< Callback function to be called when the timer expires
	void				*arg;		   //!< User-defined argument passed to the callback function
	int					 is_periodic;  //!< Flag indicating if the timer is periodic
	int					 is_started;   //!< Flag indicating if the timer is started
	size_t				 period_ms;	   //!< Timer period in milliseconds
	struct pal_timer_s	*next;		   //!< Pointer to the next timer in the list
};

typedef struct pal_timer_env_s
{
	pthread_t			thread_handle;	//!< Thread handle for the timer thread
	pthread_mutex_t		mutex;			//!< Mutex for synchronizing access to the timer list
	pthread_cond_t		cond;			//!< Condition variable for signaling the timer thread
	struct pal_timer_s *timer_list;		//!< Pointer to the list of timers
	int					shutdown_flag;	//!< Flag indicating if the timer thread should shut down
} pal_timer_env_t;

extern pal_timer_env_t pal_timer_environment;  //!< Global timer environment structure

// ============================
// Function Declarations
// ============================
/**
 * @brief Initialize the timer module
 * @return 0 on success, -1 on failure
 */
int pal_timer_init(void);

/**
 * @brief Deinitialize the timer module
 */
void pal_timer_deinit(void);

/**
 * @brief Compare two timespec structures
 * @param a Pointer to the first timespec structure
 * @param b Pointer to the second timespec structure
 * @return 0 if equal, negative value if a < b, positive value if a > b
 */
int pal_os_timer_time_cmp(const struct timespec *a, const struct timespec *b);

/**
 * @brief Insert a timer into the sorted timer list
 * @param timer Pointer to the timer to be inserted
 */
void pal_os_timer_insert_sorted(pal_timer_t *timer);

/**
 * @brief Remove a timer from the sorted timer list
 * @param timer Pointer to the timer to be inserted
 */
void pal_os_timer_remove(pal_timer_t *timer);

/**
 * @brief Timer thread function
 * @param arg Pointer to user-defined argument passed to the thread function
 * @return NULL
 * @note This function is intended to be used as the entry point for the timer thread.
 */
void *pal_timer_thread_fn(void *arg);

#ifdef __cplusplus
}
#endif
