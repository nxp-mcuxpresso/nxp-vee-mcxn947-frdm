/**
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file
 * @brief NXP GPIO low level API
 * @author Julien Jouan
 * @version 1.0.0
 */

#include <LLGPIO_impl.h>
#include "LLGPIO_mux.h"
#include "sni.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_clock.h"

static int8_t check_port_and_pin(uint32_t port, uint32_t pin)
{
    int8_t ret = kPinError;

    switch (port)
    {
        case GPIO00:
            /* According to MCXN947 RM, all pins available on this bank */
            ret = kSuccess;
            break;
        case GPIO01:
            /* According to MCXN947 RM, only pins 0-23 and 30-31 available on this bank */
            if (pin <= 23 || pin >= 30) ret = kSuccess;
            break;
        case GPIO02:
            /* According to MCXN947 RM, only pins 0-11 available on this bank */
            if (pin <= 11) ret = kSuccess;
            break;
        case GPIO03:
            /* According to MCXN947 RM, only pins 0-23 available on this bank */
            if (pin <= 23) ret = kSuccess;
            break;
        case GPIO04:
            /* According to MCXN947 RM, only pins 0-7 and 12-23 available on this bank */
            if (pin <= 7 || (pin >= 12 && pin <= 23)) ret = kSuccess;
            break;
        case GPIO05:
            /* According to MCXN947 RM, only pins 0-9 available on this bank */
            if (pin <= 9) ret = kSuccess;
            break;
        default:
            ret = kPortError;
            break;
    }
    return ret;
}

int32_t LLGPIO_IMPL_init_gpio_pin(uint32_t pin, uint32_t direction, uint32_t pullConfig)
{
    uint32_t port = pin / GPIO_BANK_SIZE;
    pin %= GPIO_BANK_SIZE;

    /* Check that GPIO port and pin used is existing */
    int8_t portPinStatus = check_port_and_pin(port, pin);
    if (portPinStatus == kSuccess)
    {
        CLOCK_EnableClock(CLK_GATE_DEFINE(AHB_CLK_CTRL0, 13 + port));
        CLOCK_EnableClock(CLK_GATE_DEFINE(AHB_CLK_CTRL0, 19 + port));

        const port_pin_config_t portx_piny_config = {/* Internal pull-up/down resistor is disabled */
                                                    ((pullConfig > 0) ? ((pullConfig == 1) ? kPORT_PullUp : kPORT_PullDown) : kPORT_PullDisable),
                                                    /* Low internal pull resistor value is selected. */
                                                    kPORT_LowPullResistor,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive input filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain output is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as PIO0_10 */
                                                    kPORT_MuxAlt0,
                                                    /* Digital input enabled */
                                                    kPORT_InputBufferEnable,
                                                    /* Digital input is not inverted */
                                                    kPORT_InputNormal,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};

        PORT_SetPinConfig(PORT_ARRAY[port], pin, &portx_piny_config);
        GPIO_ARRAY[port]->PDDR =  ((GPIO_ARRAY[port]->PDDR) & ~(1U << pin)) | (direction << pin);
    }
    return portPinStatus;
}

int32_t LLGPIO_IMPL_read_gpio_pin(uint32_t pin)
{
    uint32_t port = pin / GPIO_BANK_SIZE;
    pin %= GPIO_BANK_SIZE;

    /* Check that GPIO port and pin used is existing */
    int8_t portPinStatus = check_port_and_pin(port, pin);
    int8_t ret;

    if (portPinStatus == kSuccess)
    {
        ret = GPIO_PinRead(GPIO_ARRAY[port], pin);
    }
    else
    {
        ret = portPinStatus;
    }

    return ret;
}

int32_t LLGPIO_IMPL_write_gpio_pin(uint32_t pin, uint32_t level)
{
    uint32_t port = pin / GPIO_BANK_SIZE;
    pin %= GPIO_BANK_SIZE;

    /* Check that GPIO port and pin used is existing */
    int8_t portPinStatus = check_port_and_pin(port, pin);
    int8_t ret;

    if (portPinStatus == kSuccess)
    {
        GPIO_PinWrite(GPIO_ARRAY[port], pin, level);
        ret = GPIO_PinRead(GPIO_ARRAY[port], pin);
    }
    else
    {
        ret = portPinStatus;
    }

    return ret;
}

int32_t LLGPIO_IMPL_toggle_gpio_pin(uint32_t pin)
{
    uint32_t port = pin / GPIO_BANK_SIZE;
    pin %= GPIO_BANK_SIZE;

    /* Check that GPIO port and pin used is existing */
    int8_t portPinStatus = check_port_and_pin(port, pin);
    int8_t ret;

    if (portPinStatus == kSuccess)
    {
        GPIO_PortToggle(GPIO_ARRAY[port], (uint32_t) (1 << pin));
        ret = GPIO_PinRead(GPIO_ARRAY[port], pin);
    }
    else
    {
        ret = portPinStatus;
    }

    return ret;
}
