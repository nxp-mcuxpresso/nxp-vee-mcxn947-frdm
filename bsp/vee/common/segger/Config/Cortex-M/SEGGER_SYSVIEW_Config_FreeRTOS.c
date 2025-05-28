/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2015 - 2017  SEGGER Microcontroller GmbH & Co. KG        *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       SEGGER SystemView * Real-time application analysis           *
*                                                                    *
**********************************************************************
*                                                                    *
* All rights reserved.                                               *
*                                                                    *
* SEGGER strongly recommends to not make any changes                 *
* to or modify the source code of this software in order to stay     *
* compatible with the RTT protocol and J-Link.                       *
*                                                                    *
* Redistribution and use in source and binary forms, with or         *
* without modification, are permitted provided that the following    *
* conditions are met:                                                *
*                                                                    *
* o Redistributions of source code must retain the above copyright   *
*   notice, this list of conditions and the following disclaimer.    *
*                                                                    *
* o Redistributions in binary form must reproduce the above          *
*   copyright notice, this list of conditions and the following      *
*   disclaimer in the documentation and/or other materials provided  *
*   with the distribution.                                           *
*                                                                    *
* o Neither the name of SEGGER Microcontroller GmbH & Co. KG         *
*   nor the names of its contributors may be used to endorse or      *
*   promote products derived from this software without specific     *
*   prior written permission.                                        *
*                                                                    *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND             *
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,        *
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF           *
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
* DISCLAIMED. IN NO EVENT SHALL SEGGER Microcontroller BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR           *
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT  *
* OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;    *
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF      *
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT          *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE  *
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH   *
* DAMAGE.                                                            *
*                                                                    *
**********************************************************************
*                                                                    *
*       SystemView version: V2.52a                                    *
*                                                                    *
**********************************************************************
*                                                                    *
* Copyright 2023 MicroEJ Corp. This file has been modified by MicroEJ Corp.  *
*                                                                    *
**********************************************************************
-------------------------- END-OF-HEADER -----------------------------

File    : SEGGER_SYSVIEW_Config_FreeRTOS.c
Purpose : Sample setup configuration of SystemView with FreeRTOS.
Revision: $Rev: 7745 $
*/
#include "FreeRTOS.h"
#include "SEGGER_SYSVIEW.h"

#include "task.h"
#include "LLMJVM_MONITOR_sysview.h"
#include "LLTRACE_sysview_configuration.h"
#include "SEGGER_SYSVIEW_Conf.h"
#include "main.h"

extern const SEGGER_SYSVIEW_OS_API SYSVIEW_X_OS_TraceAPI;

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
// The application name to be displayed in SystemViewer
#define SYSVIEW_APP_NAME        "NPA Application"

// The target device name
#define SYSVIEW_DEVICE_NAME     "Cortex-M33"

// Frequency of the timestamp. Must match SEGGER_SYSVIEW_GET_TIMESTAMP in SEGGER_SYSVIEW_Conf.h
#define SYSVIEW_TIMESTAMP_FREQ  (configCPU_CLOCK_HZ)

// System Frequency. SystemcoreClock is used in most CMSIS compatible projects.
#define SYSVIEW_CPU_FREQ        configCPU_CLOCK_HZ

// The lowest RAM address used for IDs (pointers)
#define SYSVIEW_RAM_BASE        (0x10000000)

/********************************************************************* 
*
*       _cbSendSystemDesc()
*
*  Function description
*    Sends SystemView description strings.
*/
static void _cbSendSystemDesc(void) {
  SEGGER_SYSVIEW_SendSysDesc("N="SYSVIEW_APP_NAME",D="SYSVIEW_DEVICE_NAME",O=FreeRTOS");
  SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");
}

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

SEGGER_SYSVIEW_OS_API SYSVIEW_MICROEJ_X_OS_TraceAPI;

static void SYSVIEW_MICROEJ_X_OS_SendTaskList(void){
   SYSVIEW_X_OS_TraceAPI.pfSendTaskList();

// The strategy to send tasks info is different in post mortem and live analysis.
#if (1 == SEGGER_SYSVIEW_POST_MORTEM_MODE)
   /**
   * POST MORTEM analysis
   *
   * Using the post mortem analysis, FreeRTOS tasks regularly call the SYSVIEW_MICROEJ_X_OS_SendTaskList() function when
   * a packet (systemview event) is sent to the SEGGER circular buffer. It is necessary because the information of tasks
   * must be regularly uploaded in the circular buffer in order to provide a valid analysis at any moment.
   * Consequently, we only allow to call LLMJVM_MONITOR_SYSTEMVIEW_send_task_list() when the current task is the MicroEJ Core Engine.
   */

   /* Obtain the handle of the current task. */
   TaskHandle_t xHandle = xTaskGetCurrentTaskHandle();
   configASSERT( xHandle ); // Check the handle is not NULL.

   // Check if the current task handle is the MicroEJ Core Engine task handle. pvMicroEJVeeCreatedTask is an external variable.
   if( xHandle == pvMicroEJVeeCreatedTask){
      // Launched by the MicroEJ Core Engine, we execute LLMJVM_MONITOR_SYSTEMVIEW_send_task_list()
      LLMJVM_MONITOR_SYSTEMVIEW_send_task_list();
   }
#else
   /**
   * LIVE analysis
   *
   * Using the live analysis, the call of SYSVIEW_MICROEJ_X_OS_SendTaskList() is triggered by
   * the SystemView Software through the J-Link probe. Consequently, the MicroEJ Core Engine task will never call
   * the function LLMJVM_MONITOR_SYSTEMVIEW_send_task_list(). However, if the MicroEJ Core Engine task is created,
   * the function must be called LLMJVM_MONITOR_SYSTEMVIEW_send_task_list().
   */
   // Check if the MicroEJ Core Engine task handle is not NULL. pvMicroEJVeeCreatedTask is an external variable.
   if( NULL != pvMicroEJVeeCreatedTask){
      // The MicroEJ Core Engine task is running, we execute LLMJVM_MONITOR_SYSTEMVIEW_send_task_list()
      LLMJVM_MONITOR_SYSTEMVIEW_send_task_list();
   }
#endif
}

void SEGGER_SYSVIEW_Conf(void) {
   SYSVIEW_MICROEJ_X_OS_TraceAPI.pfGetTime = SYSVIEW_X_OS_TraceAPI.pfGetTime;
   SYSVIEW_MICROEJ_X_OS_TraceAPI.pfSendTaskList = SYSVIEW_MICROEJ_X_OS_SendTaskList;

   SEGGER_SYSVIEW_Init(SYSVIEW_TIMESTAMP_FREQ, SYSVIEW_CPU_FREQ, &SYSVIEW_MICROEJ_X_OS_TraceAPI, _cbSendSystemDesc);
   SEGGER_SYSVIEW_SetRAMBase(SYSVIEW_RAM_BASE);
}

/*************************** End of file ****************************/
