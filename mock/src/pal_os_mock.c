/*
 * File: pal_os_mock.c
 * Description: Mocking of pal_os library.
 * Author: Massimiliano Ianniello
 */

#include "pal_os_mock.h"

/* ---------------------------------------------------------------------------
 * Type Definitions
 * ---------------------------------------------------------------------------
 */
/* ---------------------------------------------------------------------------
 * Static Definitions
 * ---------------------------------------------------------------------------
 */
/* ---------------------------------------------------------------------------
 * Macros
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Constants
 * ---------------------------------------------------------------------------
 */
/* ---------------------------------------------------------------------------
 * Variables
 * ---------------------------------------------------------------------------
 */
/* ---------------------------------------------------------------------------
 * Static Functions
 * ---------------------------------------------------------------------------
 */
/* ---------------------------------------------------------------------------
 * Function Implementations
 * ---------------------------------------------------------------------------
 */
DEFINE_FAKE_VALUE_FUNC(int, pal_mutex_create, pal_mutex_t *, int)
DEFINE_FAKE_VALUE_FUNC(int, pal_mutex_lock, pal_mutex_t *, size_t)
DEFINE_FAKE_VALUE_FUNC(int, pal_mutex_unlock, pal_mutex_t *)
DEFINE_FAKE_VALUE_FUNC(int, pal_mutex_destroy, pal_mutex_t *)

DEFINE_FAKE_VALUE_FUNC(int, pal_queue_create, pal_queue_t *, size_t, size_t)
DEFINE_FAKE_VALUE_FUNC(int, pal_queue_enqueue, pal_queue_t *, void *const, size_t)
DEFINE_FAKE_VALUE_FUNC(int, pal_queue_dequeue, pal_queue_t *, void *const, size_t)
DEFINE_FAKE_VOID_FUNC(pal_queue_reset, pal_queue_t *)
DEFINE_FAKE_VALUE_FUNC(size_t, pal_queue_get_free_slots, pal_queue_t *)
DEFINE_FAKE_VALUE_FUNC(size_t, pal_queue_get_items, pal_queue_t *)
DEFINE_FAKE_VOID_FUNC(pal_queue_destroy, pal_queue_t *)

DEFINE_FAKE_VALUE_FUNC(int, pal_signal_create, pal_signal_t *)
DEFINE_FAKE_VALUE_FUNC(pal_signal_ret_code_t, pal_signal_wait, pal_signal_t *, size_t, size_t *, int, int, size_t)
DEFINE_FAKE_VALUE_FUNC(int, pal_signal_set, pal_signal_t *, size_t)
DEFINE_FAKE_VALUE_FUNC(int, pal_signal_clear, pal_signal_t *, size_t)
DEFINE_FAKE_VALUE_FUNC(int, pal_signal_destroy, pal_signal_t *)

DEFINE_FAKE_VOID_FUNC_VARARG(pal_system_printf, const char *, ...)
DEFINE_FAKE_VALUE_FUNC(pal_system_t *, pal_system_get_stats)

DEFINE_FAKE_VALUE_FUNC(int, pal_thread_create, pal_thread_t *, pal_thread_priority_t, size_t, pal_thread_func_t, const char *, void *)
DEFINE_FAKE_VOID_FUNC(pal_thread_sleep, size_t)
DEFINE_FAKE_VOID_FUNC(pal_thread_scheduler_start)
DEFINE_FAKE_VOID_FUNC(pal_thread_join, pal_thread_t *const)
DEFINE_FAKE_VALUE_FUNC(const char *, pal_thread_get_name, pal_thread_t *const)
DEFINE_FAKE_VALUE_FUNC(size_t, pal_thread_get_stack_watermark, pal_thread_t *const)
DEFINE_FAKE_VOID_FUNC(pal_thread_free, pal_thread_t *)

DEFINE_FAKE_VALUE_FUNC(size_t, pal_get_unix_time)
DEFINE_FAKE_VOID_FUNC(pal_set_unix_time, size_t)
DEFINE_FAKE_VALUE_FUNC(size_t, pal_get_system_time)

DEFINE_FAKE_VALUE_FUNC(int, pal_timer_create, pal_timer_t *, const char *, pal_timer_type_t, size_t, pal_timer_callback_t, int, void *)
DEFINE_FAKE_VALUE_FUNC(int, pal_timer_start, pal_timer_t *)
DEFINE_FAKE_VALUE_FUNC(int, pal_timer_stop, pal_timer_t *)
DEFINE_FAKE_VALUE_FUNC(int, pal_timer_restart, pal_timer_t *)
DEFINE_FAKE_VALUE_FUNC(int, pal_timer_change_period, pal_timer_t *, size_t)
DEFINE_FAKE_VALUE_FUNC(int, pal_is_timer_active, pal_timer_t *)
DEFINE_FAKE_VALUE_FUNC(int, pal_timer_delete, pal_timer_t *)