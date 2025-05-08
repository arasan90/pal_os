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

// ============================
// Function Declarations
// ============================
/**
 * @brief Get the current time in seconds since the epoch (1970-01-01 00:00:00 UTC).
 * @return The current time in seconds since the epoch.
 */
size_t pal_get_unix_time(void);

/**
 * @brief Get the current time in milliseconds since the system started.
 * @return The current time in milliseconds since the system started.
 */
size_t pal_get_system_time(void);

#ifdef __cplusplus
}
#endif
