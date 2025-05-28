/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 * Copyright 2023 MicroEJ Corp. This file has been modified by MicroEJ Corp.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

#ifndef _PANEL_FUNC_H_
#define _PANEL_FUNC_H_

#include <stdint.h>
#include "lcdic_panel_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_LCD_BIG_ENDIAN    0
#define APP_LCD_LITTLE_ENDIAN 1

#if (defined(APP_PANEL_ADAFRUIT_2_8_CAPTATIVE) && APP_PANEL_ADAFRUIT_2_8_CAPTATIVE)
#define APP_LCD_HEIGHT 240u
#define APP_LCD_WIDTH  320u


#ifdef I8080_SCREEN
  #define APP_LCDIC_MODE kLCDIC_I8080
  #define APP_LCDIC_8080_FLAG \
      ((uint8_t)kLCDIC_I8080_CsActiveLow | (uint8_t)kLCDIC_I8080_DcCmdLow | \
                           (uint8_t)kLCDIC_I8080_RdActiveLow | (uint8_t)kLCDIC_I8080_WrActiveLow | \
                           (uint8_t)kLCDIC_I8080_CsEnableIdleOff)
#else
  #define APP_LCDIC_MODE kLCDIC_4WireSPI
  #define APP_LCDIC_SPI_FLAG \
    (kLCDIC_SPI_MsbFirst | kLCDIC_SPI_ClkActiveLow | kLCDIC_SPI_ClkPhaseSecondEdge | kLCDIC_SPI_DcCmdLow)
      
#endif

#define APP_LCDI_ENDIAN APP_LCD_BIG_ENDIAN

#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

void APP_InitPanel(void);

void APP_PanelSelectRegion(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY);

void APP_InitFlexioMcuLcd(void);
#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _PANEL_FUNC_H_ */
