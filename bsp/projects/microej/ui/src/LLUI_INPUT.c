/**
 * Copyright 2022-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include "LLUI_INPUT_impl.h"
#include "microej.h"
#include "touch_manager.h"
#include "interrupts.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "LLUI_INPUT_impl.h"

static xSemaphoreHandle g_sem_input;

/* API -----------------------------------------------------------------------*/

void LLUI_INPUT_IMPL_initialize(void)
{
	g_sem_input = xSemaphoreCreateBinary();
	xSemaphoreGive(g_sem_input);

	TOUCH_MANAGER_initialize();
}

int32_t LLUI_INPUT_IMPL_getInitialStateValue(int32_t stateMachinesID, int32_t stateID)
{
	// no state on this BSP
	return 0;
}

void LLUI_INPUT_IMPL_enterCriticalSection()
{
	if (interrupt_is_in() == MICROEJ_FALSE)
	{
		xSemaphoreTake(g_sem_input, portMAX_DELAY);
	}
}

void LLUI_INPUT_IMPL_leaveCriticalSection()
{
	if (interrupt_is_in() == MICROEJ_FALSE)
	{
		xSemaphoreGive(g_sem_input);
	}
}
