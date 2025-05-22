/*
 * File: signals.c
 * Description: Implementation of signals-related functionality for the freeRTOS platform.
 * Author: Massimiliano Ianniello
 */

#include "pal_os/signal.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
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
 * Static Functions
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Function Implementations
 * ---------------------------------------------------------------------------
 */
int pal_signal_create(pal_signal_t *signal)
{
	int ret_code = -1;
	if (signal)
	{
		*signal	 = (pal_signal_t)xEventGroupCreate();
		ret_code = (*signal) ? 0 : -1;
	}
	return ret_code;
}

pal_signal_ret_code_t pal_signal_wait(pal_signal_t *signal, size_t mask, size_t *received_signals, int clear_mask, int wait_all, size_t timeout_ms)
{
	pal_signal_ret_code_t ret_code = PAL_SIGNAL_FAILURE;
	if (signal && received_signals)
	{
		TickType_t	timeout_ticks = portMAX_DELAY;
		EventBits_t bits		  = 0;
		if (PAL_OS_INFINITE_TIMEOUT != timeout_ms)
		{
			timeout_ticks = pdMS_TO_TICKS(timeout_ms);
		}
		bits = xEventGroupWaitBits((EventGroupHandle_t)*signal, mask, clear_mask, wait_all, timeout_ticks);
		if ((wait_all && (bits == mask)) || !wait_all && (bits & mask))
		{
			ret_code = PAL_SIGNAL_SUCCESS;
		}
		else
		{
			ret_code = PAL_SIGNAL_TIMEOUT;
		}
		*received_signals = bits;
	}
	return ret_code;
}

int pal_signal_set(pal_signal_t *signal, size_t mask, int from_isr)
{
	int ret_code = -1;
	if (signal)
	{
		if (from_isr)
		{
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			if (pdPASS == xEventGroupSetBitsFromISR((EventGroupHandle_t)*signal, mask, &xHigherPriorityTaskWoken))
			{
				if (xHigherPriorityTaskWoken)
				{
					portYIELD_FROM_ISR();
				}
				ret_code = 0;
			}
		}
		else
		{
			if (pdPASS == xEventGroupSetBits((EventGroupHandle_t)*signal, mask))
			{
				ret_code = 0;
			}
		}
	}
	return ret_code;
}

int pal_signal_clear(pal_signal_t *signal, size_t mask)
{
	int ret_code = -1;
	if (signal)
	{
		ret_code = (pdPASS == xEventGroupClearBits((EventGroupHandle_t)*signal, mask)) ? 0 : -1;
	}
	return ret_code;
}

int pal_signal_destroy(pal_signal_t **signal)
{
	int ret_code = -1;
	if (signal)
	{
		vEventGroupDelete((EventGroupHandle_t)*signal);
		ret_code = 0;
	}
	return ret_code;
}