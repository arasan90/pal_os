/*
 * File: system.c
 * Description: Implementation of system-related functionality for the Linux platform.
 * Author: Massimiliano Ianniello
 */

#include "pal_os/system.h"

#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
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
static pal_system_t pal_system_stats = {0};

/* ---------------------------------------------------------------------------
 * Static Functions
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Function Implementations
 * ---------------------------------------------------------------------------
 */
void pal_system_printf(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

pal_system_t *pal_system_get_stats(void) { return &pal_system_stats; }

void *pal_system_malloc(size_t size)
{
	void *ptr = malloc(size);
	if (NULL != ptr)
	{
		pal_system_stats.used_heap_size += malloc_usable_size(ptr);
		pal_system_stats.max_used_heap_size =
			(pal_system_stats.used_heap_size > pal_system_stats.max_used_heap_size) ? pal_system_stats.used_heap_size : pal_system_stats.max_used_heap_size;
	}
	return ptr;
}

void *pal_system_calloc(size_t num, size_t size)
{
	void *ptr = calloc(num, size);
	if (NULL != ptr)
	{
		pal_system_stats.used_heap_size += malloc_usable_size(ptr);
		pal_system_stats.max_used_heap_size =
			(pal_system_stats.used_heap_size > pal_system_stats.max_used_heap_size) ? pal_system_stats.used_heap_size : pal_system_stats.max_used_heap_size;
	}
	return ptr;
}

void *pal_system_realloc(void *ptr, size_t size)
{
	size_t old_size = 0;
	if (NULL != ptr)
	{
		old_size = malloc_usable_size(ptr);
	}
	ptr = realloc(ptr, size);
	if (NULL != ptr)
	{
		pal_system_stats.used_heap_size += (malloc_usable_size(ptr) - old_size);
		pal_system_stats.max_used_heap_size =
			(pal_system_stats.used_heap_size > pal_system_stats.max_used_heap_size) ? pal_system_stats.used_heap_size : pal_system_stats.max_used_heap_size;
	}
	return ptr;
}

void pal_system_free(void *ptr)
{
	if (NULL != ptr)
	{
		pal_system_stats.used_heap_size -= malloc_usable_size(ptr);
		free(ptr);
	}
}