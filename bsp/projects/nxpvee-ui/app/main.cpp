/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "LLMJVM.h"
#include "LLBSP_impl.h"
#include "sni.h"

#include "fsl_clock.h"

#include "sdcard_helper.h"
#ifdef ENABLE_NET
#include "fsl_phylan8741.h"
#endif

#ifdef ENABLE_SEC
#include <assert.h>
#include "els_pkc_mbedtls.h"
#endif

#include "tree_version.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Task priorities. */
#define nxp_pa_task_PRIORITY (configMAX_PRIORITIES - 6)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void nxp_pa_task(void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
int main(void)
{
    /* Init board hardware. */
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach FRO 12M to FLEXCOMM2 */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom2Clk, 1u);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);

    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 1u);
    CLOCK_AttachClk(kPLL0_to_FLEXIO);
    BOARD_InitBootClocks();
    BOARD_I2C_ReleaseBus();
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();

    /* Init smartdma. */
    BOARD_InitSmartDMA();

#ifdef ENABLE_NET
    /* Init Ethernet clock */
    CLOCK_AttachClk(kNONE_to_ENETRMII);
    CLOCK_EnableClock(kCLOCK_Enet);
    SYSCON0->PRESETCTRL2 = SYSCON_PRESETCTRL2_ENET_RST_MASK;
    SYSCON0->PRESETCTRL2 &= ~SYSCON_PRESETCTRL2_ENET_RST_MASK;
#endif

#ifdef ENABLE_SEC
    /* used in order to initialize the true random number generator TRNG */
    /* The CRYPTO_InitHardware is responsible for initializing various hardware components related to cryptography according to options configured */
    status_t status = CRYPTO_InitHardware() ;
    assert( status == kStatus_Success );
#endif

#ifdef SD_ENABLED
    START_SDCARD_Task(NULL);
#endif

    if (xTaskCreate(nxp_pa_task, "NXP_PA_task", 5*1024, NULL, nxp_pa_task_PRIORITY, NULL) !=
        pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }
    vTaskStartScheduler();
    for (;;)
        ;
}


void Java_ej_microvg_VectorGraphicsNatives_initialize() { }
void Java_ej_microvg_MatrixNatives_identity() { }

void microjvm_main(void) {
       void* vm;
       int32_t err;
       int32_t exitcode;

       // create VM
       vm = SNI_createVM();

       if (vm == NULL) {
               PRINTF("VM initialization error.\n");
       } else {
               PRINTF("VM START\n");
               err = SNI_startVM(vm, 0, NULL);

               if (err < 0) {
                       // Error occurred
                       if (err == LLMJVM_E_EVAL_LIMIT) {
                               PRINTF("Evaluation limits reached.\n");
                       } else {
                               PRINTF("VM execution error (err = %d).\n", -err);
                       }
               } else {
                       // VM execution ends normally
                       exitcode = SNI_getExitCode(vm);
                       PRINTF("VM END (exit code = %d)\n", exitcode);
               }

               // delete VM
               SNI_destroyVM(vm);
       }
}


/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
extern char _HeapAsFreeRAMSize __asm("_HeapAsFreeRAMSize");
static void nxp_pa_task(void *pvParameters)
{
    for (;;)
    {
        PRINTF("\r\nNXP PLATFORM ACCELERATOR\r\n");
        PRINTF("NXP VEE Port '%s' '%s'\r\n", VEE_VERSION, GIT_SHA_1);
        PRINTF("NXP VEE Heap size: %u Bytes\r\n", (unsigned)&_HeapAsFreeRAMSize);

        microjvm_main();
        vTaskSuspend(NULL);
    }
}
void vApplicationMallocFailedHook()
{
    PRINTF(("\r\nERROR: Malloc failed to allocate memory\r\n"));

    /* Loop forever */
    for (;;)
        ;
}

/*!
 * @brief Call the cpuload_idle function in FreeRTOS ilde hook function
 */

extern "C" void vApplicationIdleHook(void)
{
}


/* Functions used in Core Validation test */
#include "sni.h"
extern "C" jfloat Java_com_microej_core_tests_MicroejCoreValidation_testFloat (jfloat a, jfloat b)
{
    return a * b;
}

extern "C" jdouble Java_com_microej_core_tests_MicroejCoreValidation_testDouble (jdouble a, jdouble b)
{
    return a * b;
}

#ifndef ENABLE_NET
/* need to define this to avoid missing symbol */
extern "C" int32_t Java_com_is2t_support_net_natives_ChannelNatives_initialize(void) { return 0; }
#endif
