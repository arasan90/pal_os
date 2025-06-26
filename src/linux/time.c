/*
 * File: time.c
 * Description: Implementation of time-related functionality for the Linux platform.
 * Author: Massimiliano Ianniello
 */

#include "pal_os/time.h"

#include <time.h>

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
 * Static Functions
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Function Implementations
 * ---------------------------------------------------------------------------
 */

size_t pal_get_unix_time(void)
{
	size_t			unix_time = -1;
	struct timespec ts;
	if (0 == clock_gettime(CLOCK_REALTIME, &ts))
	{
		unix_time = (size_t)(ts.tv_sec + ts.tv_nsec / 1000000000);
	}
	return unix_time;
}

size_t pal_get_system_time(void)
{
	size_t			system_time = -1;
	struct timespec ts;
	if (0 == clock_gettime(CLOCK_MONOTONIC, &ts))
	{
		system_time = (size_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
	}
	return system_time;
}

size_t pal_get_system_time_from_isr(void)
{
	size_t			system_time = -1;
	struct timespec ts;
	if (0 == clock_gettime(CLOCK_MONOTONIC, &ts))
	{
		system_time = (size_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
	}
	return system_time;
}

void pal_set_unix_time(size_t unix_time)
{
	// This function is not implemented for Linux.
	(void)unix_time;
	return;
}