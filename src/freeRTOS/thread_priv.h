#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

// ============================
// Includes
// ============================
#include "esp_heap_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/portable.h"
#include "freertos/task.h"
#include "pal_os/thread.h"

// ============================
// Macros and Constants
// ============================

// ============================
// Type Definitions
// ============================

// ============================
// Function Declarations
// ============================
/**
 * @brief Convert a priority from pal value to freeRTOS value
 * @note Defaults priorities are at 1/3, 1/2 and 2/3 of configMAX_PRIORITIES
 * @return freeRTOS value
 */
UBaseType_t pal_thread_convert_priority(pal_thread_priority_t pal_priority);

/**
 * @brief Thread function wrapper.
 * @param arg Pointer to the thread structure.
 * @return NULL.
 */
void pal_thread_generic_func(void *const arg);

#ifdef __cplusplus
}
#endif
