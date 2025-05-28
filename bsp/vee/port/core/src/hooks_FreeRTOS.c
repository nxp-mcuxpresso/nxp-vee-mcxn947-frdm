/*
 * C
 *
 * Copyright 2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/*
 * This file implements the application hooks functions needed by FreeRTOS.
 * These functions trace problems that occurs during execution.
 * Their names are defined by FreeRTOS
 * and must be implemented (here) or desactivated (in FreeRTOSConfig.h).
 *
 * See "Hook Functions [More Advanced]" on FreeRTOS Website.
 * http://www.freertos.org/a00016.hmtl
 */

/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Freescale includes. */
#include "fsl_debug_console.h"

#ifdef __cplusplus
    extern "C" {
#endif

/* Public functions ----------------------------------------------------------*/

void vApplicationStackOverflowHook(TaskHandle_t xTask, char * pcTaskName) {
    if (pcTaskName != NULL) {
        PRINTF("A stack overflow had occurred in task \"%s\"\n", pcTaskName);
    } else {
        PRINTF("A stack overflow had occurred in task\n");
    }

    while(1)
    {} // Trap when a stack overflow occurs
}

void vApplicationMallocFailedHook(void) {
    PRINTF("A malloc fail had occurred\n");

    while(1)
    {} // Trap when a call to malloc fails
}

#ifdef __cplusplus
    }
#endif
