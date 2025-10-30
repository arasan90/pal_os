/*
 * File: timer.c
 * Description: Implementation of timer-related functionality for the freeRTOS platform.
 * Author: Massimiliano Ianniello
 */

#include "pal_os/timer.h"

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
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

/* ---------------------------------------------------------------------------
 * Static Functions
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Function Implementations
 * ---------------------------------------------------------------------------
 */

int pal_timer_create(pal_timer_t *timer, const char *name, pal_timer_type_t type, size_t period, pal_timer_callback_t callback, int auto_start, void *arg)
{
	int ret_code = -1;
	if (timer && callback && period)
	{
		BaseType_t auto_reload = (PAL_TIMER_TYPE_PERIODIC == type) ? pdTRUE : pdFALSE;
		*timer				   = (pal_timer_t *)xTimerCreate(name, pdMS_TO_TICKS(period), auto_reload, arg, (TimerCallbackFunction_t)callback);
		if (*timer)
		{
			ret_code = 0;
			if (auto_start)
			{
				xTimerStart((TimerHandle_t)*timer, portMAX_DELAY);
			}
		}
	}
	return ret_code;
}

int pal_timer_start(pal_timer_t *timer)
{
	int ret_code = -1;
	if (timer)
	{
		xTimerStart((TimerHandle_t)timer, portMAX_DELAY);
		ret_code = 0;
	}
	return ret_code;
}

PAL_OS_RAM_ATTR int pal_timer_start_from_isr(pal_timer_t *timer)
{
	int ret_code = -1;
	if (timer)
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		if (pdPASS == xTimerStartFromISR((TimerHandle_t)timer, &xHigherPriorityTaskWoken))
		{
			if (xHigherPriorityTaskWoken)
			{
				portYIELD_FROM_ISR();
			}
			ret_code = 0;
		}
	}
	return ret_code;
}

int pal_timer_stop(pal_timer_t *timer)
{
	int ret_code = -1;
	if (timer)
	{
		xTimerStop((TimerHandle_t)timer, portMAX_DELAY);
		ret_code = 0;
	}
	return ret_code;
}

PAL_OS_RAM_ATTR int pal_timer_stop_from_isr(pal_timer_t *timer)
{
	int ret_code = -1;
	if (timer)
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		if (pdPASS == xTimerStopFromISR((TimerHandle_t)timer, &xHigherPriorityTaskWoken))
		{
			if (xHigherPriorityTaskWoken)
			{
				portYIELD_FROM_ISR();
			}
			ret_code = 0;
		}
	}
	return ret_code;
}

int pal_timer_restart(pal_timer_t *timer)
{
	int ret_code = -1;
	if (timer)
	{
		xTimerReset((TimerHandle_t)timer, portMAX_DELAY);
		ret_code = 0;
	}
	return ret_code;
}

PAL_OS_RAM_ATTR int pal_timer_restart_from_isr(pal_timer_t *timer)
{
	int ret_code = -1;
	if (timer)
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		if (pdPASS == xTimerResetFromISR((TimerHandle_t)timer, &xHigherPriorityTaskWoken))
		{
			if (xHigherPriorityTaskWoken)
			{
				portYIELD_FROM_ISR();
			}
			ret_code = 0;
		}
	}
	return ret_code;
}

int pal_timer_change_period(pal_timer_t *timer, size_t new_period)
{
	int ret_code = -1;
	if (timer && new_period)
	{
		xTimerChangePeriod((TimerHandle_t)timer, new_period, portMAX_DELAY);
		ret_code = 0;
	}
	return ret_code;
}

PAL_OS_RAM_ATTR int pal_timer_change_period_from_isr(pal_timer_t *timer, size_t new_period)
{
	int ret_code = -1;
	if (timer && new_period)
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		if (pdPASS == xTimerChangePeriodFromISR((TimerHandle_t)timer, new_period, &xHigherPriorityTaskWoken))
		{
			if (xHigherPriorityTaskWoken)
			{
				portYIELD_FROM_ISR();
			}
			ret_code = 0;
		}
	}
	return ret_code;
}

int pal_is_timer_active(pal_timer_t *timer)
{
	int ret_code = 0;
	if (timer)
	{
		ret_code = pdTRUE == xTimerIsTimerActive((TimerHandle_t)timer) ? 1 : 0;
	}
	return ret_code;
}

int pal_timer_delete(pal_timer_t *timer)
{
	int ret_code = -1;
	if (timer)
	{
		xTimerDelete((TimerHandle_t)*timer, portMAX_DELAY);
		ret_code = 0;
	}
	return ret_code;
}