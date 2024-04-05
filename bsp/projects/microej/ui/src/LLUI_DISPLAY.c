/*
 * C
 *
 * Copyright 2018-2023 MicroEJ Corp. All rights reserved.
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

/****************************UI PORT FUNCTION***************************************/

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
}

static DISPLAY_CONFIGURATION_flush_region flush_region;

uint8_t* LLUI_DISPLAY_IMPL_flush(MICROUI_GraphicsContext* gc,
                                 uint8_t* srcAddr, uint32_t xmin,
                                 uint32_t ymin, uint32_t xmax,
                                 uint32_t ymax)
{
#ifdef HARD_FLUSH_DBG
    PRINTF("x_offset: %d\r\n", flush_region.x_offset);
    PRINTF("y_offset: %d\r\n", flush_region.y_offset);
    PRINTF("width: %d\r\n", flush_region.width);
    PRINTF("height: %d\r\n", flush_region.height);
#endif

    xmin = flush_region.x_offset;
    ymin = flush_region.y_offset;
    xmax = xmin + flush_region.width - 1;
    ymax = ymin + flush_region.height - 1;

    uint32_t number_pixels = flush_region.width * flush_region.height;

#ifdef HARD_FLUSH_DBG
    PRINTF("xmin: %d\r\n", xmin);
    PRINTF("ymin: %d\r\n", ymin);
    PRINTF("xmax: %d\r\n", xmax);
    PRINTF("ymax: %d\r\n", ymax);
    PRINTF("\r\n");
#endif

    disp_sup_flush(srcAddr, xmin, ymin, xmax, ymax, number_pixels);

    LLUI_DISPLAY_flushDone(false);
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

void Java_com_microej_partial_support_PartialBufferNatives_setFlushLimits(jint xOffset, jint yOffset,
                                                                          jint width, jint height)
{
    flush_region.x_offset = xOffset;
    flush_region.y_offset = yOffset;
    flush_region.width = width;
    flush_region.height = height;
}

int32_t Java_com_microej_partial_support_PartialBufferNatives_getBufferHeight()
{
    return LCD_VIRTUAL_BUF_HEIGHT;
}
