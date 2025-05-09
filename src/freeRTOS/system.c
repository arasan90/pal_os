/*
 * File: system.c
 * Description: Implementation of system-related functionality for the freeRTOS platform.
 * Author: Massimiliano Ianniello
 */

#include "pal_os/system.h"

#include <stdarg.h>
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/portable.h"

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

pal_system_t *pal_system_get_stats(void)
{
	pal_system_stats.free_heap_size		= xPortGetFreeHeapSize();
	pal_system_stats.min_free_heap_size = xPortGetMinimumEverFreeHeapSize();
	return &pal_system_stats;
}