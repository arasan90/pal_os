/*
 * File: time.c
 * Description: Implementation of time-related functionality for the Linux platform.
 * Author: Massimiliano Ianniello
 */

#include "pal_os/time.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pal_os/common.h"

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
static size_t pal_unix_time = 0;

/* ---------------------------------------------------------------------------
 * Static Functions
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Function Implementations
 * ---------------------------------------------------------------------------
 */

size_t pal_get_unix_time(void) { return pal_unix_time; }

size_t pal_get_system_time(void) { return (pdTICKS_TO_MS(xTaskGetTickCount())); }

PAL_OS_RAM_ATTR size_t pal_get_system_time_from_isr(void) { return (pdTICKS_TO_MS(xTaskGetTickCountFromISR())); }

void pal_set_unix_time(size_t unix_time) { pal_unix_time = unix_time; }