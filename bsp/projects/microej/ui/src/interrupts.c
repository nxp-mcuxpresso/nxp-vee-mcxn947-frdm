/*
 * C
 *
 * Copyright 2015-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */


/* Includes ------------------------------------------------------------------*/

#include "interrupts.h"
#include "FreeRTOS.h"

/* Public functions ----------------------------------------------------------*/

uint8_t interrupt_is_in(void)
{
	BaseType_t is_in_ISR;

	is_in_ISR = xPortIsInsideInterrupt();
	if (is_in_ISR == pdTRUE)
	{
		return MICROEJ_TRUE;
	}
	else
	{
		return MICROEJ_FALSE;
	}
}
