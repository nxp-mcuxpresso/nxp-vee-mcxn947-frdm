/*
 * C
 *
 * Copyright 2015-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */


/* Includes ------------------------------------------------------------------*/

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "touch_helper.h"
#include "touch_manager.h"
#include "microej.h"
#include "display_support.h"
#include "fsl_debug_console.h"

// -----------------------------------------------------------------------------
// Macros and Defines
// -----------------------------------------------------------------------------

#define BOARD_TOUCH_I2C     Driver_I2C2
#define BOARD_TOUCH_I2C_IRQ FLEXCOMM2_IRQn

/*
 * @brief Task delay (in ms) between detecting a multiple pressed events
 */
#define TOUCH_DELAY 30

/*
 * @brief Task stack size
 */
#define TOUCH_STACK_SIZE  (1024)
#define TOUCH_TASK_PRIORITY ( 9 )

// -----------------------------------------------------------------------------
// Static Variables
// -----------------------------------------------------------------------------

//static ft6x06_handle_t touch_handle;

/*
 * @brief vglite operation semaphore
 */
#if 0
static SemaphoreHandle_t touch_interrupt_sem;
#endif

/* Private API ---------------------------------------------------------------*/

/**
 * Touch thread routine
 */
static void __touch_manager_task(void *pvParameters);


/* API -----------------------------------------------------------------------*/

void TOUCH_MANAGER_initialize(void)
{
    DEMO_InitTouch();

#define TOUCH_PRIORITY (5)

	/* Create the touch screen task */
	if (pdPASS != xTaskCreate(__touch_manager_task, "Touch screen task", TOUCH_STACK_SIZE, NULL, TOUCH_PRIORITY, NULL)) {
		PRINTF("Touch task initialization failed\r\n");
		assert(false);
	}
}

void TOUCH_MANAGER_interrupt(void)
{
	// use busy-way mechanism for the time being
#if 0
	uint32_t intStat = GPIO_PortGetInterruptStatus(GPIO, TOUCH_INTERRUPT_PORT, kGPIO_InterruptA);
	if (intStat & (1UL << TOUCH_INTERRUPT_PIN))
	{
		GPIO_PortClearInterruptFlags(GPIO, TOUCH_INTERRUPT_PORT, kGPIO_InterruptA,
				(1UL << TOUCH_INTERRUPT_PIN));

		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(touch_interrupt_sem, &xHigherPriorityTaskWoken);
		if(xHigherPriorityTaskWoken != pdFALSE )
		{
			// Force a context switch here.
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
	}
#endif
}

// -----------------------------------------------------------------------------
// Internal functions
// -----------------------------------------------------------------------------


// See the section 'Internal function definitions' for the function documentation
static void __touch_manager_read(void)
{
	//touch_event_t touch_event;
	int pressed = -1;
	int touch_x;
	int touch_y;

	DEMO_ReadTouch(&pressed, &touch_x, &touch_y);
	if (pressed == 0)
	{
#ifdef TOUCH_DBG
		PRINTF("touch_manager_read %d [%d %d]\r\n", pressed, touch_x, touch_y);
#endif
		TOUCH_HELPER_pressed(touch_x, touch_y);
	}
	else if (pressed == 1)
	{
#ifdef TOUCH_DBG
		PRINTF("touch_manager_read %d [%d %d]\r\n", pressed, touch_x, touch_y);
#endif
		TOUCH_HELPER_released();
	}
}

// See the section 'Internal function definitions' for the function documentation
static void __touch_manager_task(void *pvParameters)
{
	while (1)
	{
		/* Suspend ourselves */
		//xSemaphoreTake(touch_interrupt_sem, portMAX_DELAY);

		/* We have been woken up, lets work ! */
		__touch_manager_read();
		vTaskDelay(TOUCH_DELAY);
	}
}
