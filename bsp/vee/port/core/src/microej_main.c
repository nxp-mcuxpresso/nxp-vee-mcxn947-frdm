/*
 * C
 *
 *  Copyright 2020-2022 MicroEJ Corp. All rights reserved.
 *  This library is provided in source code for use, modification and test, subject to license terms.
 *  Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 *
 */

/**
 * @file
 * @brief MicroEJ startup.
 * @author MicroEJ Developer Team
 * @version 2.0.0
 * @date 15 September 2020
 */

#include "fsl_debug_console.h"
#include "microej_main.h"
#include "LLMJVM.h"
#include "sni.h"

#ifdef __cplusplus
    extern "C" {
#endif

/**
 * @brief Creates and starts a MicroEJ instance. This function returns when the MicroEJ execution ends.
 */
void microej_main(int argc, char **argv) {
	void* vm;
	int32_t err;
	int32_t exitcode;

	// create VM
	vm = SNI_createVM();

	if (vm == NULL) {
		PRINTF("MicroEJ initialization error.\n");
	} else {
		PRINTF("MicroEJ START\n");

		// Error codes documentation is available in LLMJVM.h
		err = SNI_startVM(vm, argc, argv);

		if (err < 0) {
			// Error occurred
			if (err == LLMJVM_E_EVAL_LIMIT) {
				PRINTF("Evaluation limits reached.\n");
			} else {
				PRINTF("MicroEJ execution error (err = %d).\n", (int) err);
			}
		} else {
			// VM execution ends normally
			exitcode = SNI_getExitCode(vm);
			PRINTF("MicroEJ END (exit code = %d)\n", (int) exitcode);
		}

		// delete VM
		SNI_destroyVM(vm);
	}
}

#ifdef __cplusplus
    }
#endif
