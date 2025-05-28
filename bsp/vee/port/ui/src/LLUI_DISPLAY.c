/*
 * C
 *
 * Copyright 2018-2024 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Includes ------------------------------------------------------------------*/

#include <string.h>
#include <stdbool.h>
#include "lcdic_panel_config.h"
#include "LLUI_DISPLAY_impl.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "sni.h"
#include "lcd.h"


#include "fsl_common.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "display_configuration.h"
#include "display_support.h"
#include "fsl_adapter_gpio.h"

// -----------------------------------------------------------------------------
// Macros and Defines
// -----------------------------------------------------------------------------

#define DISPLAY_TASK_PRIORITY    (tskIDLE_PRIORITY + 5)
#define DISPLAY_TASK_STACK_SIZE  (1024)

#define BOARD_TE_GPIO       (GPIO0)
#define BOARD_TE_GPIO_PORT  (0)
#define BOARD_TE_GPIO_PIN   (13)
#define BOARD_TE_INTERRUPT_TYPE kHAL_GpioInterruptRisingEdge

GPIO_HANDLE_DEFINE(s_LcdTEHandle);
// -----------------------------------------------------------------------------
// Global Variables
// -----------------------------------------------------------------------------

static DISPLAY_CONFIGURATION_flush_region flush_region;

/*
 * @brief: Semaphore to synchronize the display flush with MicroUI
 */
static SemaphoreHandle_t sync_flush;

/*
 * @brief: Back buffer address.
 */
static uint8_t* back_buffer_address;

/*
 * @brief: Flag to know if the flush has to be synchronized on Tearing Effect.
 * 1 if flush has to be synchronize with Tearing Effect. 
 * 0 if flush has to be performed immediately.
 */
static uint8_t is_synchronized_on_tearing;

/**
 * @brief Flag to synchronize the flush on the Tearing Effect interrupt.
 * 1 if a copy has to be performed.
 * 0 if no copy has to be performed.
*/
static uint8_t start_copy;

// -----------------------------------------------------------------------------
// Private functions
// -----------------------------------------------------------------------------

/*
 * @brief: Task to manage display flushes.
 */
static void __display_task(void * pvParameters) {
    uint32_t x_min = 0;
    uint32_t y_min = 0;
    uint32_t x_max = 0;
    uint32_t y_max = 0;
    uint32_t number_pixels = 0;

    while (1)
    {
        xSemaphoreTake(sync_flush, portMAX_DELAY);

    #ifdef HARD_FLUSH_DBG
        PRINTF("x_offset: %d\r\n", flush_region.x_offset);
        PRINTF("y_offset: %d\r\n", flush_region.y_offset);
        PRINTF("width: %d\r\n", flush_region.width);
        PRINTF("height: %d\r\n", flush_region.height);
    #endif

        x_min = flush_region.x_offset;
        y_min = flush_region.y_offset;
        x_max = x_min + flush_region.width - 1;
        y_max = y_min + flush_region.height - 1;

        number_pixels = flush_region.width * flush_region.height;

    #ifdef HARD_FLUSH_DBG
        PRINTF("xmin: %d\r\n", xmin);
        PRINTF("ymin: %d\r\n", ymin);
        PRINTF("xmax: %d\r\n", xmax);
        PRINTF("ymax: %d\r\n", ymax);
        PRINTF("\r\n");
    #endif

        disp_sup_flush(back_buffer_address, x_min, y_min, x_max, y_max, number_pixels);

        LLUI_DISPLAY_flushDone(false);
    }
}

void BOARD_SW_IRQ_HANDLER(void);

// -----------------------------------------------------------------------------
// LLUI_DISPLAY_impl.h functions
// -----------------------------------------------------------------------------

void LLUI_DISPLAY_IMPL_initialize(LLUI_DISPLAY_SInitData* init_data)
{
    // Initialize context data.
    init_data->back_buffer_address = disp_sup_get_fb_address();
    init_data->lcd_height = LCD_HEIGHT;
    init_data->lcd_width = LCD_WIDTH;

    init_data->binary_semaphore_0 = (LLUI_DISPLAY_binary_semaphore*) xSemaphoreCreateBinary();
    init_data->binary_semaphore_1 = (LLUI_DISPLAY_binary_semaphore*) xSemaphoreCreateBinary();

    disp_sup_pre_init();
    disp_sup_disp_init();
    disp_sup_indev_init();

    back_buffer_address = disp_sup_get_fb_address();

    sync_flush = xSemaphoreCreateBinary();
    if(pdPASS != xTaskCreate(__display_task, "Display", DISPLAY_TASK_STACK_SIZE, NULL, DISPLAY_TASK_PRIORITY, NULL)){
        PRINTF("Display task initialization failed\r\n");
		assert(false);
    }

    // Enable the tearing interrupt.
    start_copy = 0;
    hal_gpio_pin_config_t te_sw_config = {
            kHAL_GpioDirectionIn,
            0,
            BOARD_TE_GPIO_PORT,
            BOARD_TE_GPIO_PIN,
    };
    HAL_GpioInit(s_LcdTEHandle, &te_sw_config);
    HAL_GpioSetTriggerMode(s_LcdTEHandle, BOARD_TE_INTERRUPT_TYPE);
    HAL_GpioInstallCallback(s_LcdTEHandle, BOARD_SW_IRQ_HANDLER, NULL);

}

uint8_t* LLUI_DISPLAY_IMPL_flush(MICROUI_GraphicsContext* gc,
                                 uint8_t* srcAddr, uint32_t xmin,
                                 uint32_t ymin, uint32_t xmax,
                                 uint32_t ymax)
{
    if(is_synchronized_on_tearing) {
        // First flush of the new drawings, flush need to be synchronized on the Tearing Effect.

        // Unlock the job of the tearing interrupt.
        start_copy = 1;

        // First flush of the new drawings is done.
        is_synchronized_on_tearing = 0;
    } else {
        // First flush of the drawings was synchronized. Flush as fast as possible the remaining part of the drawings.

        // Wake up display task.
        xSemaphoreGive(sync_flush);
    }

    return srcAddr;
}

void LLUI_DISPLAY_IMPL_binarySemaphoreTake(void* sem){
    xSemaphoreTake((xSemaphoreHandle)sem, portMAX_DELAY);
}

void LLUI_DISPLAY_IMPL_binarySemaphoreGive(void* sem, bool under_isr) {
    if (under_isr) {
        portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR((xSemaphoreHandle)sem, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken != pdFALSE) {
            // Force a context switch here.
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
    else {
        xSemaphoreGive((xSemaphoreHandle)sem);
    }
}

// Implement Interrupt handler.
void BOARD_SW_IRQ_HANDLER(void)
{
    if(start_copy) {
        start_copy = 0;
        
        // Wake up display task.
        xSemaphoreGive(sync_flush);
    }
}



// -----------------------------------------------------------------------------
// PartialBufferNatives implementations.
// -----------------------------------------------------------------------------

/*
 * @brief Sets the bounds of the rectangle area that should be flushed during the following
 * <code>LLUI_DISPLAY_IMPL_flush</code> calls.
 * <p>
 * The x and y offsets indicate at which position the back buffer should be flushed within the frame buffer.
 * <p>
 * The platform driver implementation of <code>LLUI_DISPLAY_IMPL_flush</code> function has to add an offset (managed
 * by the policy) to <code>xmin/ymin/xmax/ymax</code> for the <b>destination buffer</b> (frame buffer). The source
 * buffer bounds stay cropped to <code>xmin/ymin/xmax/ymax</code>.
 *
 * @param[in] xOffset the x offset of the area.
 * @param[in] yOffset the y offset of the area.
 * @param[in] width the width of the area.
 * @param[in] height the height of the area.
 *
 */
void Java_com_microej_partial_support_PartialBufferNatives_setFlushLimits(jint xOffset, jint yOffset,
                                                                          jint width, jint height)
{
    flush_region.x_offset = xOffset;
    flush_region.y_offset = yOffset;
    flush_region.width = width;
    flush_region.height = height;
}

/*
 * @brief Returns the height of the partial buffer allocated by the platform.
 * 
 * @return the height of the partial buffer (in pixels).
 */
int32_t Java_com_microej_partial_support_PartialBufferNatives_getBufferHeight()
{
    return LCD_VIRTUAL_BUF_HEIGHT;
}

/*
 * @brief Notifies native side that a flush is starting.
 */
void Java_com_microej_partial_support_PartialBufferNatives_startFlush(void)
{
    is_synchronized_on_tearing = 1;
}
