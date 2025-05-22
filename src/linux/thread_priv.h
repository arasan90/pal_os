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
 * @brief Thread function wrapper.
 * @param arg Pointer to the thread structure.
 * @return NULL.
 */
void *pal_thread_generic_func(void *const arg);

#ifdef __cplusplus
}
#endif
