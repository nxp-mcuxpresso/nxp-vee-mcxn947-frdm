/*
 * Copyright 2024 NXP
 *
 * Copyright 2024 MicroEJ Corp. This file has been modified by MicroEJ Corp.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LVGL_SUPPORT_H
#define LVGL_SUPPORT_H

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef BOARD_USE_FLEXIO_SMARTDMA
#define BOARD_USE_FLEXIO_SMARTDMA 1
#endif

#ifndef BOARD_LCD_S035
#define BOARD_LCD_S035 0
#endif

#if BOARD_LCD_S035
#define LCD_WIDTH  480
#define LCD_HEIGHT 320
#else
#define LCD_WIDTH  800
#define LCD_HEIGHT 480
#endif

#define LCD_FB_BYTE_PER_PIXEL 2
/* The virtual buffer for DBI panel */
#if LCD_VIRTUAL_BUF_HEIGHT_DIVIDER
#define LCD_VIRTUAL_BUF_HEIGHT ((LCD_HEIGHT) / (LCD_VIRTUAL_BUF_HEIGHT_DIVIDER))
#else
// By default, divide the LCD Buffer height by 4.
#define LCD_VIRTUAL_BUF_HEIGHT ((LCD_HEIGHT) / 4)
#endif
#define LCD_VIRTUAL_BUF_SIZE  ((LCD_WIDTH) * (LCD_VIRTUAL_BUF_HEIGHT))

/* LCD panel. */
#define BOARD_LCD_RST_GPIO GPIO4
#define BOARD_LCD_RST_PIN  7
#define BOARD_LCD_TE_GPIO  GPIO0
#define BOARD_LCD_TE_PIN   13
#define BOARD_LCD_CS_GPIO  GPIO0
#define BOARD_LCD_CS_PIN   12
#define BOARD_LCD_RS_GPIO  GPIO0
#define BOARD_LCD_RS_PIN   7
#define BOARD_LCD_INT_PORT PORT4
#define BOARD_LCD_INT_GPIO GPIO4
#define BOARD_LCD_INT_PIN  6

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void disp_sup_pre_init(void);
void disp_sup_disp_init(void);
void disp_sup_indev_init(void);
void disp_sup_flush(uint8_t* srcAddr, uint32_t startX, uint32_t startY, uint32_t endX, uint32_t endY, uint32_t number_pixel);
uint8_t* disp_sup_get_fb_address();
void DEMO_InitTouch(void);
void DEMO_ReadTouch(int *pressed, int *touch_x, int *touch_y);

#if defined(__cplusplus)
}
#endif

#endif /*LVGL_SUPPORT_H */
