/*
 * Copyright 2024 NXP
 * Copyright 2025 MicroEJ Corp. This file has been modified by MicroEJ Corp.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <string.h>
#include "fsl_adapter_reset.h"
#include "sni.h"
#include "LLDEVICE_impl.h"
#include "tree_version.h"

/* Defines -------------------------------------------------------------------*/

#define as_string_int(x) #x
#define as_string(x) as_string_int(x)

#define DEVICE_ARCHITECTURE_NAME "NXP-MCXN947-FRDM"

/* Public functions ----------------------------------------------------------*/

/**
 * Fills the given buffer with the name of the architecture. The buffer is filled with a null-terminated string.
 * @param buffer the buffer to fill with the name of the architecture.
 * @param length the size of the buffer.
 * @return 1 on success or 0 on error.
 */
uint8_t LLDEVICE_IMPL_getArchitecture(uint8_t* buffer, int32_t length) {
	uint8_t ret = 0;

	if (length > strlen((DEVICE_ARCHITECTURE_NAME))) {
		(void)strncpy((char *)buffer, (DEVICE_ARCHITECTURE_NAME), strlen((DEVICE_ARCHITECTURE_NAME)) + 1);
		ret = 1;
	}

	return ret;
}

/**
 * Fills the given buffer with the ID of the device.
 * @param buffer the buffer to fill with the ID.
 * @param length the size of the buffer.
 * @return the number of bytes filled or 0 on error.
 */
uint32_t LLDEVICE_IMPL_getId(uint8_t* buffer, int32_t length)
{
    static const char * board_name = as_string(SDK_BOARD_ID);
    int how_many = (length < strlen(board_name)) ? (length) : (strlen(board_name));
    strncpy((char *)buffer, board_name, how_many);
    return how_many;
}

/**
 * Fills the given buffer with the Version of the device as
 * a null-terminated string.
 *
 * @param buffer the buffer to fill with the Version.
 */
void LLDEVICE_IMPL_getVersion(uint8_t* buffer) {
		(void)strncpy((char *)buffer, (VEE_VERSION), strlen((VEE_VERSION)) + 1);
}

/**
 * Reboots the device.
 */
void LLDEVICE_IMPL_reboot(void)
{
	__disable_irq();
	HAL_ResetMCU();
}

/**
 * Shutdowns the device.
 */
void LLDEVICE_IMPL_shutdown(void)
{
	(void)SNI_throwNativeException(-1, "Not implemented");
}
