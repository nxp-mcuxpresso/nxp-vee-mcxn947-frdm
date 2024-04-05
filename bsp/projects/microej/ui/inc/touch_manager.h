/*
 * C
 *
 * Copyright 2015-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */


#ifndef _TOUCH_MANAGER
#define _TOUCH_MANAGER

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

#define TOUCH_INTERRUPT_PORT              0U
#define TOUCH_INTERRUPT_PIN               10U
#define APP_GPIO_INTA_IRQHandler GPIO_INTA_DriverIRQHandler
#define TOUCH_INTERRUPT_IRQ               GPIO_INTA_IRQn
#define TOUCH_INTERRUPT_CONNECTED_LEVEL   0U

/* API -----------------------------------------------------------------------*/

void TOUCH_MANAGER_initialize(void);
void TOUCH_MANAGER_interrupt(void);

#endif
