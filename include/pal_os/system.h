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
	size_t heap_size;			//!< Total heap size in bytes. @note Not implemented in linux
	size_t used_heap_size;		//!< Used heap size in bytes.
	size_t free_heap_size;		//!< Free heap size in bytes. @note Not implemented in linux
	size_t max_used_heap_size;	//!< Maximum used heap size in bytes.
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

/**
 * @brief Allocates a block of memory of the specified size.
 *
 * @param size The size of the memory block to allocate, in bytes.
 * @return A pointer to the allocated memory block, or NULL if the allocation fails.
 */
void *pal_system_malloc(size_t size);

/**
 * @brief Allocates memory for an array of elements and initializes all bytes to zero.
 *
 * @param num The number of elements to allocate.
 * @param size The size of each element, in bytes.
 * @return A pointer to the allocated and zero-initialized memory block, or NULL if the allocation fails.
 */
void *pal_system_calloc(size_t num, size_t size);

/**
 * @brief Reallocates a previously allocated memory block to a new size.
 *
 * @param ptr A pointer to the previously allocated memory block. If NULL, this function behaves like malloc().
 * @param size The new size of the memory block, in bytes. If size is 0, the memory block is freed.
 * @return A pointer to the reallocated memory block, or NULL if the reallocation fails.
 */
void *pal_system_realloc(void *ptr, size_t size);

/**
 * @brief Frees a previously allocated memory block.
 *
 * @param ptr A pointer to the memory block to free. If ptr is NULL, no operation is performed.
 */
void pal_system_free(void *ptr);

#ifdef __cplusplus
}
#endif
