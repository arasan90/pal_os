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
typedef struct pal_system_s
{
	size_t free_heap_size;		//!< Free heap size in bytes. @note Not implemented in linux
	size_t min_free_heap_size;	//!< Maximum used heap size in bytes. @note Not implemented in linux
} pal_system_t;

// ============================
// Function Declarations
// ============================
/**
 * @brief Outputs a formatted string to the system's standard output.
 * This function behaves similarly to the standard printf function, allowing
 * formatted output using a format string and a variable number of arguments.
 *
 * @param format A null-terminated format string that specifies how subsequent
 *               arguments are converted for output. The format string can
 *               include format specifiers such as %d, %s, %f, etc.
 * @param ...    A variable number of arguments to be formatted and printed
 *               according to the format string.
 */
void pal_system_printf(const char *format, ...);

/**
 * @brief Retrieves system statistics.
 * This function provides access to system-heap statistics.
 *
 * @return A pointer to a `pal_system_t` structure containing the system statistics.
 *         The caller should ensure proper handling of the returned pointer.
 * @note Not all information are reliable on all platforms (e.g. on Linux it is not possible to have a valid heap size)
 */
pal_system_t *pal_system_get_stats(void);

#ifdef __cplusplus
}
#endif
