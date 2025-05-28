/*
 * C
 *
 * Copyright 2015-2024 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/* Includes ------------------------------------------------------------------*/

#include "cpuload_impl.h"
#include "fsl_debug_console.h"

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef CPULOAD_DEBUG
// cppcheck-suppress misra-c2012-21.6 // Include only in debug
#include <stdio.h>
#define CPULOAD_DEBUG_TRACE(...) (void)PRINTF(__VA_ARGS__)
#else
#define CPULOAD_DEBUG_TRACE(...) ((void)0)
#endif

/* Globals -------------------------------------------------------------------*/

#ifdef CPULOAD_ENABLED
static volatile uint32_t cpuload_schedule_time = 0;
static volatile uint32_t cpuload_reference_counter = 0;
static volatile uint32_t cpuload_idle_counter = 0;
static volatile uint32_t cpuload_idle_periods = 0;
static volatile uint32_t cpuload_idle_time = 0;
#endif

/* API -----------------------------------------------------------------------*/

void cpuload_idle(void)
{
#ifdef CPULOAD_ENABLED
	__disable_irq();
	cpuload_idle_counter++;
	__enable_irq();
#endif
}


int32_t cpuload_init(void)
{
#ifdef CPULOAD_ENABLED

	int32_t return_code = CPULOAD_OK;

	// get reference time (must be done before creating task)
	return_code = cpuload_impl_start_idle_task();
	if (return_code == CPULOAD_OK) {
		cpuload_impl_sync_os_tick();
		cpuload_idle_counter = 0;
		cpuload_impl_sleep(CPULOAD_SCHEDULE_TIME/CPULOAD_SUBSAMPLING_PERIOD_RATIO);

		if (cpuload_idle_counter == (uint32_t)0){
			// it is an error: this counter must have been updated
			// during the previous sleep.
			CPULOAD_DEBUG_TRACE("CPU load startup: invalid idle counter value: %u\n", cpuload_idle_counter);
			return_code = CPULOAD_INVALID_COUNTER;
		} else {

		// fix globals
		cpuload_schedule_time = CPULOAD_SCHEDULE_TIME;
		cpuload_reference_counter = (cpuload_idle_counter*(uint32_t)CPULOAD_SUBSAMPLING_PERIOD_RATIO)+(uint32_t)(CPULOAD_SUBSAMPLING_PERIOD_RATIO/2);
		cpuload_idle_counter = 0;
		cpuload_idle_periods = 0;
		cpuload_idle_time = 0;

		return_code = cpuload_impl_start_task();
		}
	}

	return return_code;

#else
	return CPULOAD_NOT_ENABLED;
#endif
}

uint32_t cpuload_get(void)
{
#ifdef CPULOAD_ENABLED
	uint32_t cpu_load = 0;
	if(cpuload_idle_periods)
	{
		cpu_load = (uint32_t)100 - (((uint32_t )100 * cpuload_idle_time) / (cpuload_idle_periods * cpuload_reference_counter));
		__disable_irq();
		cpuload_idle_periods = cpuload_idle_time = 0;
		__enable_irq();
	}
	return cpu_load;
#else
	return 0;
#endif
}

void cpuload_task(void)
{
#ifdef CPULOAD_ENABLED
	while(1)
	{
		// sleep during n milliseconds
		cpuload_impl_sleep(cpuload_schedule_time);

		__disable_irq();
		// increase cpu idle period
		cpuload_idle_periods++;
		// store cpu idle time
		cpuload_idle_time += cpuload_idle_counter;
		__enable_irq();

		// reset cpuload counter
		cpuload_idle_counter = 0;
	}
#endif
}


/* Java API ------------------------------------------------------------------*/

uint32_t javaCPULoadInit(void)
{
	return cpuload_init();
}

uint32_t javaCPULoadGet(void)
{
	return cpuload_get();
}


#ifdef __cplusplus
    }
#endif