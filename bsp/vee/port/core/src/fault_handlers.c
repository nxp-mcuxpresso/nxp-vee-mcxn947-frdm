/*
 * C
 *
 * Copyright 2014-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 *
 * Copyright 2024 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 * @file fault_handlers.c
 *
 * @brief This file contains the fault exception handlers.
 *
 * @details The CMSIS names are used.
 *
 * There are two modes:
 * - verbose
 * - lite
 *
 * In verbose mode, each fault exception handler will:
 * - print the stack frame,
 * - print its name,
 * - print the associated fault status register,
 * - try to make an analysis of the fault cause,
 * - make an infinite loop.
 *
 * In lite mode, each fault exception handler will:
 * - print its name,
 * - make an infinite loop.
 *
 * Define the @ref VERBOSE_MODE macro to activate verbose mode.
 */

/* Includes ______________________________________________________________________*/
#include <stdbool.h>
#include <stdio.h>

#include "fsl_debug_console.h"

#include "fault_handlers.h" // will check prototypes and import public types


typedef struct __attribute__((packed)) ContextStateFrame {
  uint32_t r0;
  uint32_t r1;
  uint32_t r2;
  uint32_t r3;
  uint32_t r12;
  uint32_t lr;
  uint32_t return_address;
  uint32_t xpsr;
} sContextStateFrame;



// NOTE: If you are using CMSIS, the registers can also be
// accessed through CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk
#define HALT_IF_DEBUGGING()                              \
  do {                                                   \
    if ((*(volatile uint32_t *)0xE000EDF0) & (1 << 0)) { \
      __asm("bkpt 1");                                   \
    }                                                    \
} while (0)

/* Macros ________________________________________________________________________*/

/**
 * @brief Define this macro to if you are using Keil's ARMCC toolchain. If you are using IAR or GCC toolchains, do not define it.
 */
//#define COMPILE_WITH_ARMCC

/**
 * @brief Define this macro to enable verbose handlers. Verbose handlers will consume  more memory than lite handlers.
 */
#define VERBOSE_MODE

/**
 * @brief Make an infinite while loop.
 */
#define INFINITE_LOOP()		{while(1);}

#ifdef VERBOSE_MODE
/**
 * @brief This macro is called as the begining of each handlers to print the stack frame.
 *
 * @details There are two stack pointers in Cortex-M processors:
 * - MSP (Main Stack Pointer)
 * - PSP (Process Stack Pointer)
 * When handling faults, we need to know which one was used before entering the faults so that
 * we can retrieve interesting information.
 *
 * To do so, we need to test the value of EXC_RETURN, which is stored into LR during exception
 * (see Section 8.1.4 EXC_RETURN). Then, we can fetch the correct stack pointer with the MRS instruction
 * (MRS = Move from special register to general register).
 *
 * NOTE: IAR's compiler doesn't like then comments are interlaced with macro definition lines.
 * Here is what the macro does:
 * - Check if bit 2 (stack return) is set in EXC_RETURN / LR
 * - If-Then condition instruction
 * - If equals, store MSP in R0
 * - Else, store PSP in RO (1st parameter)
 * - MSP is stored in R1 (2nd parameter)
 * - PSP is stored in R2 (3rd parameter)
 * - LR is stored in R3 (4th parameter)
 * - Call C function for printing
 */
#if defined(__CC_ARM)
void printer(uint32_t current_sp, uint32_t msp, uint32_t psp, uint32_t exc_return);

__asm void print_stacked_registers()
{
    TST     LR, #4
    ITE     EQ
    MRSEQ   R0, MSP
    MRSNE   R0, PSP
    MRS     R1, MSP
    MRS     R2, PSP
    MOV     R3, LR
	  LDR     R4, =__cpp(printer)
	  BX		  R4
}
#else
// Compatible with GCC and IAR
#define print_stacked_registers() {\
    __asm("TST LR, #4");\
    __asm("MRS R1, MSP");\
    __asm("MRS R2, PSP");\
    __asm("MOV R3, LR");\
    __asm("BL printer");\
}
#endif

// CMSIS doesn't provide macro to get bits from BFSR (which is the 2nd byte in CFSR), so we create our own macros
#define BFSR_IBUSERR_Pos        (0)
#define BFSR_IBUSERR_Msk        (1 << BFSR_IBUSERR_Pos)

#define BFSR_PRECISERR_Pos      (1)
#define BFSR_PRECISERR_Msk      (1 << BFSR_PRECISERR_Pos)

#define BFSR_IMPRECISERR_Pos    (2)
#define BFSR_IMPRECISERR_Msk    (1 << BFSR_IMPRECISERR_Pos)

#define BFSR_BFARVALID_Pos      (15)
#define BFSR_BFARVALID_Msk      (1 << BFSR_BFARVALID_Pos)

// CMSIS doesn't provide macro to get bits from UFSR (which is the upper half-word in CFSR), so we create our own macros
#define UFSR_UNDEFINSTR_Pos        (0)
#define UFSR_UNDEFINSTR_Msk        (1 << UFSR_UNDEFINSTR_Pos)

#define UFSR_INVSTATE_Pos          (1)
#define UFSR_INVSTATE_Msk          (1 << UFSR_INVSTATE_Pos)


/* Global variables  ___________________________________________________________*/


/* Private types and variables  ___________________________________________________*/


/* Private functions  ______________________________________________________________*/
// WARNING:
/**
 * @brief C function to print the stacked registers (== the stack frame) along with EXEC_RETURN.
 *
 * @warning this function cannot be static because it won't be seen by assembly code during link edition.
 * You should not called this function directly.
 */
/*static*/ void printer(uint32_t current_sp, uint32_t msp, uint32_t psp, uint32_t exc_return)
{
	  uint32_t *sp;
    PRINTF("---------------------------------------------------------------------\n");
    // Show stack pointers
    PRINTF("Current SP = %.8lX\n", current_sp);
    PRINTF("MSP = %.8lX\n", msp);
    PRINTF("PSP = %.8lX\n", psp);
    PUTCHAR('\n');

    // Show stacked registers == stack frame (see section 8.1.3 and figure 12.4)
    sp = (uint32_t*) current_sp;
    PRINTF("Stack frame:\n");
    PRINTF("R0 =\t%.8lX\n", *sp++);
    PRINTF("R1 =\t%.8lX\n", *sp++);
    PRINTF("R2 =\t%.8lX\n", *sp++);
    PRINTF("R3 =\t%.8lX\n", *sp++);
    PRINTF("R12 =\t%.8lX\n", *sp++);
    PRINTF("LR =\t%.8lX\n", *sp++);
    PRINTF("PC =\t%.8lX\n", *sp++);
    PRINTF("xPSR =\t%.8lX\n", *sp++);
    PUTCHAR('\n');

    PRINTF("EXC_RETURN (LR) = %.8lX\n", exc_return);

    PRINTF("---------------------------------------------------------------------\n");
}


#define HARDFAULT_HANDLING_ASM(_x)               \
  __asm volatile(                                \
      "tst lr, #4 \n"                            \
      "ite eq \n"                                \
      "mrseq r0, msp \n"                         \
      "mrsne r0, psp \n"                         \
      "movw r1, #0xED00 \n"                  \
      "movt r1, #0xE000 \n"                  \
      "b my_fault_handler_c \n"                  \
                                                 )


// Disable optimizations for this function so "frame" argument
// does not get optimized away
#ifdef __ICCARM__
#pragma optimize=none
#endif
#ifdef __GNUC__
void __attribute__ ((noinline)) my_fault_handler_c(sContextStateFrame *frame, SCB_Type * scb) {
#else
void my_fault_handler_c(sContextStateFrame *frame, SCB_Type * scb) {
#endif
  // If and only if a debugger is attached, execute a breakpoint
  // instruction so we can take a look at what triggered the fault
  HALT_IF_DEBUGGING();

  	uint32_t cfsr = SCB->CFSR;
	uint32_t hfsr = SCB->HFSR;
	uint32_t mmfar = SCB->MMFAR;
	uint32_t bfar = SCB->BFAR;
      PRINTF("HardFault:\n");
      PRINTF("SCB->CFSR   0x%08lx\n", cfsr);
      PRINTF("SCB->HFSR   0x%08lx\n", hfsr);
      PRINTF("SCB->MMFAR  0x%08lx\n", mmfar);
      PRINTF("SCB->BFAR   0x%08lx\n", bfar);
      PUTCHAR('\n');
      HALT_IF_DEBUGGING();
      INFINITE_LOOP();

  // Logic for dealing with the exception. Typically:
  //  - log the fault which occurred for postmortem analysis
  //  - If the fault is recoverable,
  //    - clear errors and return back to Thread Mode
  //  - else
  //    - reboot system
}


/* Public functions  _______________________________________________________________*/

/*
 * @brief Hard Fault exception handler.
 */
void HardFault_Handler(void)
{
/*	  uint32_t hfsr;
    print_stacked_registers();
    PRINTF(__func__);

    hfsr = SCB->HFSR;
    PRINTF("Hard Fault Status Register =\t%lX\n", hfsr);

    if(hfsr & SCB_HFSR_FORCED_Msk)
    {
    	PRINTF("FORCED");
    }
*/
  HARDFAULT_HANDLING_ASM();
    INFINITE_LOOP();
}


/*
 * @brief Memory Management Fault exception handler.
 */
void MemFault_Handler (void)
{
    print_stacked_registers();
    PRINTF(__func__);

    INFINITE_LOOP();
}


/*
 * @brief Bus Fault exception handler.
 */
void BusFault_Handler (void)
{
	  uint32_t cfsr;
	  uint8_t bfsr;
	  uint32_t bfar;
	  bool still_valid;
    print_stacked_registers();
    PRINTF(__func__);

    cfsr = SCB->CFSR;
    PRINTF("Configurable Fault Status Register =\t%.8lX\n", cfsr);

    bfsr = (cfsr & SCB_CFSR_BUSFAULTSR_Msk) >> SCB_CFSR_BUSFAULTSR_Pos;
    PRINTF("Bus Fault Status Register =\t%.2X\n", bfsr);

    if(bfsr & BFSR_IBUSERR_Msk)
    {
        PRINTF("Instruction access error");
        // This case has not been experimented yet
    }

    if(bfsr & BFSR_PRECISERR_Msk)
    {
        PRINTF("Precise data access error");

        // Faulting instruction
        // --> see the PC of the printed stack frame

        // Address of faulting data access
        bfar = SCB->BFAR;
        PRINTF("Address of faulting data access (BFAR) = %.8lX\n", bfar);

        still_valid = (cfsr & BFSR_BFARVALID_Msk);
        if(!still_valid)
        {
            PRINTF("WARNING: BFAR is no longer valid!");
        }
    }

    if(bfsr & BFSR_IMPRECISERR_Msk)
    {
        PRINTF("Imprecise data access error");
    }

    INFINITE_LOOP();
}


/*
 * @brief Usage Fault exception handler.
 */
void UsageFault_Handler (void)
{
	  uint32_t cfsr;
	  uint16_t ufsr;
	// TODO Activate usage for for division by 0 and unaligned access? (see page 384)

    print_stacked_registers();
    PRINTF(__func__);

    cfsr = SCB->CFSR;
    PRINTF("Configurable Fault Status Register =\t%lX\n", cfsr);

    ufsr = (cfsr & SCB_CFSR_USGFAULTSR_Msk) >> SCB_CFSR_USGFAULTSR_Pos;
    PRINTF("Usage Fault Status Register =\t%X\n", ufsr);

    if(ufsr & UFSR_UNDEFINSTR_Msk)
    {
        PRINTF("Attempt to execute an undefined instruction");
    }

    if(ufsr & UFSR_INVSTATE_Msk)
    {
        PRINTF("Attempt to switch to invalid mode (like ARM state)");
    }

    INFINITE_LOOP();
}
#else
/*
 * @brief Hard Fault exception handler.
 */
void HardFault_Handler (void)
{
	PRINTF(__func__);
	INFINITE_LOOP();
}


/*
 * @brief Memory Management Fault exception handler.
 */
void MemFault_Handler (void)
{
	PRINTF(__func__);
	INFINITE_LOOP();
}


/*
 * @brief Bus Fault exception handler.
 */
void BusFault_Handler (void)
{
	PRINTF(__func__);
	INFINITE_LOOP();
}


/*
 * @brief Usage Fault exception handler.
 */
void UsageFault_Handler (void)
{
	PRINTF(__func__);
	INFINITE_LOOP();
}

#endif
