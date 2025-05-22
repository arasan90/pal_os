#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

// ============================
// Includes
// ============================
#include <stddef.h>
#if PAL_OS_LINUX
#include <pthread.h>
#elif PAL_OS_FREERTOS
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#endif
// ============================
// Macros and Constants
// ============================

// ============================
// Type Definitions
// ============================
/**
 * @brief Thread function.
 */
typedef void (*pal_thread_func_t)(void *arg);

/**
 * @brief Thread priority levels.
 */
typedef enum pal_thread_priority_e
{
	PAL_THREAD_PRIORITY_LOW,	 //!< Low priority
	PAL_THREAD_PRIORITY_NORMAL,	 //!< Normal priority
	PAL_THREAD_PRIORITY_HIGH,	 //!< High priority
} pal_thread_priority_t;

/**
 * @brief Thread state.
 */
typedef enum pal_thread_state_e
{
	PAL_THREAD_STATE_RUNNING,	  //!< Thread is running
	PAL_THREAD_STATE_STOPPED,	  //!< Thread is stopped
	PAL_THREAD_STATE_TERMINATED,  //!< Thread is terminated
} pal_thread_state_t;

#if PAL_OS_LINUX
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
#elif PAL_OS_FREERTOS
struct pal_thread_s
{
	TaskHandle_t	   thread_handle;		//!< POSIX thread identifier
	EventGroupHandle_t event_group_handle;	//!< Event group handle for synchronization
	pal_thread_func_t  func;				//!< Function to be executed by the thread
	void			  *arg;					//!< Argument to be passed to the thread function
	pal_thread_state_t state;				//!< Thread state
};
#endif

/**
 * @brief Thread structure.
 * This structure is opaque and should not be accessed directly.
 */
typedef struct pal_thread_s pal_thread_t;
// ============================
// Function Declarations
// ============================
/**
 * @brief Create a thread.
 *
 * @param thread Pointer to the thread structure.
 * @param priority Thread priority.
 * @param stack_size Size of the stack in bytes.
 * @param func Function to be executed by the thread.
 * @param name Thread name.
 * @note The name is used for debugging purposes and may not be supported on all platforms.
 * @note If used, the name should be a null-terminated string and should be a costant string.
 * @param arg Argument to be passed to the function.
 *
 * @return 0 on success, negative error code on failure.
 */
int pal_thread_create(pal_thread_t *thread, pal_thread_priority_t priority, size_t stack_size, pal_thread_func_t func, const char *name, void *arg);

/**
 * @brief Suspend the current thread for a specified time.
 * @param time_ms Time to sleep in milliseconds.
 */
void pal_thread_sleep(size_t time_ms);

/**
 * @brief Start the thread scheduler and block execution.
 * @note This function may not be available on all platforms.
 */
void pal_thread_scheduler_start(void);

/**
 * @brief Wait for thread pointed by thread to terminate.
 * @param thread Pointer to the thread to wait on.
 */
void pal_thread_join(pal_thread_t *const thread);

/**
 * @brief Get the name of the thread pointed by thread.
 * @param thread Pointer to the thread.
 * @return Pointer to the thread name.
 */
const char *pal_thread_get_name(pal_thread_t *const thread);

/**
 * @brief Get the maximum used stack size of the thread.
 * @param thread Pointer to the thread.
 * @return Maximum used stack size in bytes.
 */
size_t pal_thread_get_stack_watermark(pal_thread_t *const thread);

/**
 * @brief Free the thread structure.
 * @note The thread should be joined before calling this function.
 * @param thread Pointer to the thread to free.
 */
void pal_thread_free(pal_thread_t *thread);

#ifdef __cplusplus
}
#endif
