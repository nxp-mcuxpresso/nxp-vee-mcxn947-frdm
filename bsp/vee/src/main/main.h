/*
 * C
 *
 * Copyright 2024 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MAIN
#define MAIN

#ifdef __cplusplus
	extern "C" {
#endif

/* Exported variables ---------------------------------------------------*/

/**
 * @brief External FreeRTOS task handle of the MicroEJ Core Engine task.
 */
extern TaskHandle_t pvMicroEJVeeCreatedTask;

#ifdef __cplusplus
	}
#endif
#endif /* MAIN */