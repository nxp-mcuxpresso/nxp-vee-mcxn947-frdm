/*
 * FreeRTOS Kernel V10.5.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */


#ifndef PORTMACRO_H
    #define PORTMACRO_H

    #ifdef __cplusplus
        extern "C" {
    #endif

/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

/* Type definitions. */
    #define portCHAR          char
    #define portFLOAT         float
    #define portDOUBLE        double
    #define portLONG          long
    #define portSHORT         short
    #define portSTACK_TYPE    uint32_t
    #define portBASE_TYPE     long

    typedef portSTACK_TYPE   StackType_t;
    typedef long             BaseType_t;
    typedef unsigned long    UBaseType_t;


    #if ( configUSE_16_BIT_TICKS == 1 )
        typedef uint16_t     TickType_t;
        #define portMAX_DELAY              ( TickType_t ) 0xffff
    #else
        typedef uint32_t     TickType_t;
        #define portMAX_DELAY              ( TickType_t ) 0xffffffffUL

/* 32-bit tick type on a 32-bit architecture, so reads of the tick count do
 * not need to be guarded with a critical section. */
        #define portTICK_TYPE_IS_ATOMIC    1
    #endif
/*-----------------------------------------------------------*/

/* Architecture specifics. */
    #define portSTACK_GROWTH      ( -1 )
    #define portTICK_PERIOD_MS    ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
    #define portBYTE_ALIGNMENT    8
/*-----------------------------------------------------------*/


/* Scheduler utilities. */
    extern void vPortYield( void );
    #define portNVIC_INT_CTRL     ( ( volatile uint32_t * ) 0xe000ed04 )
    #define portNVIC_PENDSVSET    0x10000000
    #define portYIELD()                                 vPortYield()
    #define portEND_SWITCHING_ISR( xSwitchRequired )    if( xSwitchRequired ) *( portNVIC_INT_CTRL ) = portNVIC_PENDSVSET
    #define portYIELD_FROM_ISR( x )                     portEND_SWITCHING_ISR( x )
/*-----------------------------------------------------------*/

/* Tickless idle/low power functionality. */
#ifndef portSUPPRESS_TICKS_AND_SLEEP
    extern void vPortSuppressTicksAndSleep( TickType_t xExpectedIdleTime );
    #define portSUPPRESS_TICKS_AND_SLEEP( xExpectedIdleTime ) vPortSuppressTicksAndSleep( xExpectedIdleTime )
#endif

/*-----------------------------------------------------------*/


/* Critical section management. */

    extern void vPortEnterCritical( void );
    extern void vPortExitCritical( void );
    extern uint32_t ulSetInterruptMaskFromISR( void );
    extern void vClearInterruptMaskFromISR( uint32_t ulMask );

    #define portDISABLE_INTERRUPTS()                  __asm volatile ( "cpsid i" )
    #define portENABLE_INTERRUPTS()                   __asm volatile ( "cpsie i" )
    #define portENTER_CRITICAL()                      vPortEnterCritical()
    #define portEXIT_CRITICAL()                       vPortExitCritical()
    #define portSET_INTERRUPT_MASK_FROM_ISR()         ulSetInterruptMaskFromISR()
    #define portCLEAR_INTERRUPT_MASK_FROM_ISR( x )    vClearInterruptMaskFromISR( x )

/*-----------------------------------------------------------*/

/* Tickless idle/low power functionality. */
    #ifndef portSUPPRESS_TICKS_AND_SLEEP
        extern void vPortSuppressTicksAndSleep( TickType_t xExpectedIdleTime );
        #define portSUPPRESS_TICKS_AND_SLEEP( xExpectedIdleTime )    vPortSuppressTicksAndSleep( xExpectedIdleTime )
    #endif
/*-----------------------------------------------------------*/

/* Task function macros as described on the FreeRTOS.org WEB site. */
    #define portTASK_FUNCTION_PROTO( vFunction, pvParameters )    void vFunction( void * pvParameters )
    #define portTASK_FUNCTION( vFunction, pvParameters )          void vFunction( void * pvParameters )

    #define portNOP()

/* Suppress warnings that are generated by the IAR tools, but cannot be fixed in
 * the source code because to do so would cause other compilers to generate
 * warnings. */
    #pragma diag_suppress=Pa082

    #ifdef __cplusplus
        }
    #endif

#endif /* PORTMACRO_H */
