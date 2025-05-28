/**
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef LLGPIO_MUX
#define LLGPIO_MUX

#include "MCXN947_cm33_core0.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_PORT_NUMBER             5
#define GPIO_BANK_SIZE              32
#define GPIO00                      0
#define GPIO01                      1
#define GPIO02                      2
#define GPIO03                      3
#define GPIO04                      4
#define GPIO05                      5

typedef enum
{
    kSuccess = 0,
    kPortError = -1,
    kPinError = -2,
} gpio_init_status;

GPIO_Type *GPIO_ARRAY[] = GPIO_BASE_PTRS;

PORT_Type *PORT_ARRAY[] = PORT_BASE_PTRS;

#ifdef __cplusplus
}
#endif
#endif
