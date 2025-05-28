/*
 * C
 *
 * Copyright 2015-2024 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/*
 * Implement the *_impl* functions according the CPU/OS/Compiler
 */

#ifndef CPULOAD_IMPL
#define CPULOAD_IMPL


#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "cpuload.h"

/* API -----------------------------------------------------------------------*/

#ifdef CPULOAD_ENABLED

/*
 * CPULoad measure task. Must be called by a dedicated OS task.
 */
void cpuload_task(void);

/*
 * Create and start a OS idle task.
 * May be useless if there is already an idle routine provided by the OS.
 * Return 0 when no error
 */
int32_t cpuload_impl_start_idle_task(void);

/**
 * Wait until the next OS tick. 
 * This function ensures that we are closely synchronized with OS tick.
 * This prevents the next sleep of 1 tick takes much less time than 1 tick.
 */
void cpuload_impl_sync_os_tick(void);

/*
 * Create and start a cpuload dedicated OS task.
 * Return 0 when no error
 */
int32_t cpuload_impl_start_task(void);

/*
 * Sleep the cpuload task .
 */
void cpuload_impl_sleep(uint32_t ms);

#endif	// CPULOAD_ENABLED

#ifdef __cplusplus
    }
#endif

#endif	// CPULOAD_IMPL
