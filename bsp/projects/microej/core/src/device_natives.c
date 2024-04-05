/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <string.h>

#define as_string_int(x) #x
#define as_string(x) as_string_int(x)

uint32_t Java_ej_util_DeviceNatives_getId(uint8_t* buffer, int32_t length)
{
    static const char * board_name = as_string(SDK_BOARD_ID);
    int how_many = (length < strlen(board_name)) ? (length) : (strlen(board_name));
    strncpy((char *)buffer, board_name, how_many);
    return how_many;
}
