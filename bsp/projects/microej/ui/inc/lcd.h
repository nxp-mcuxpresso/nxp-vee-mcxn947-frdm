/**
 * Copyright 2021-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#if !defined __DISPLAY_TRANSFER_H__
#define __DISPLAY_TRANSFER_H__

#include "panel_func.h"

// ----------------------------------------------------------------------------
// Preprocessor Directives
// ----------------------------------------------------------------------------

/* Display orientation */
#define PORTRAIT 1
#define LANDSCAPE 2
#define PORTRAIT_FLIP 3
#define LANDSCAPE_FLIP 4


#define SWAP_FN(c) ((((c) & 0xff00)>>8) | (((c) & 0xff) << 8))

/* 
 * #brief Sets the default orientation mode
 * 
 * Set this preprocessing directive to:
 * - LANDSCAPE to use the landscape mode
 * - PORTRAIT to use the portrait mode
 */
#define DEFAULT_ORIENTATION_MODE    LANDSCAPE
//#define DEFAULT_ORIENTATION_MODE    PORTRAIT

/*
 * @brief Horizontal and Vertical resolution
 */ 
#if DEFAULT_ORIENTATION_MODE == LANDSCAPE
#define DISPLAY_PHYSICAL_HREZ APP_LCD_WIDTH
#define DISPLAY_PHYSICAL_VREZ APP_LCD_HEIGHT
#else
#define DISPLAY_PHYSICAL_HREZ APP_LCD_HEIGHT
#define DISPLAY_PHYSICAL_VREZ APP_LCD_WIDTH
#endif

#define DISPLAY_HREZ DISPLAY_PHYSICAL_HREZ
#define DISPLAY_VREZ DISPLAY_PHYSICAL_VREZ

/* Define size to allocate for Display buffer. (320 * 240 * 2) x 1 : 1 for BACK_BUFFER */
#define LLUI_DISPLAY_BPP 16

#define FRAME_BUFFER_HEIGHT DISPLAY_VREZ
   
#define FRAME_BUFFER_SIZE ((DISPLAY_HREZ * FRAME_BUFFER_HEIGHT * LLUI_DISPLAY_BPP) / 8)

#endif // !defined __DISPLAY_TRANSFER_H__
