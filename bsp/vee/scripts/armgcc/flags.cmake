IF(NOT DEFINED FPU)
    SET(FPU "-mfloat-abi=hard -mfpu=fpv5-sp-d16")
ENDIF()

IF(NOT DEFINED SPECS)
    SET(SPECS "--specs=nano.specs --specs=nosys.specs")
ENDIF()

IF(NOT DEFINED DEBUG_CONSOLE_CONFIG)
    SET(DEBUG_CONSOLE_CONFIG "-DSDK_DEBUGCONSOLE=1")
ENDIF()

IF(ENABLE_SHELL)
    SET(SHELL_C_FLAGS " \
    -DENABLE_SHELL=1 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DDEBUG_CONSOLE_RX_ENABLE=0 \
    -DSHELL_PRINT_COPYRIGHT=0 \
")
ENDIF()

IF(ENABLE_AI)
    SET(STACK_SZ 0x2000)
    SET(HEAP_SZ 0x2F800)
    SET(AI_C_FLAGS "-DENABLE_AI")
ELSE()
    SET(STACK_SZ 0x800)
    SET(HEAP_SZ 0x0400)
ENDIF()

IF(ENABLE_NET)
    SET(NET_C_FLAGS "-DENABLE_NET")
ENDIF()

IF(ENABLE_SEC)
    SET(SECURITY_C_FLAGS " \
    -DENABLE_SEC \
    -DMBEDTLS_MCUX_ELS_PKC_API \
    -DMBEDTLS_MCUX_USE_PKC \
    -DMBEDTLS_MCUX_ELS_API \
    -DMBEDTLS_MCUX_USE_ELS \
    -DMCUXCL_FEATURE_CSSL_MEMORY_C_FALLBACK \
")
ENDIF()

IF(ENABLE_SYSTEM_VIEW)
    SET(SYSTEM_VIEW_C_FLAGS "-DENABLE_SYSTEM_VIEW=1")
ENDIF()

IF(ENABLE_MULTI_APP)
    SET(KERNEL_WORKING_BUFFER_SIZE 0x17000)
    SET(MULTI_APP_C_FLAGS " \
    -DENABLE_MULTI_APP \
    -Xlinker \
    --defsym=__kernel_working_buffer_size__=${KERNEL_WORKING_BUFFER_SIZE} \
    -DKERNEL_WORKING_BUFFER_SIZE=${KERNEL_WORKING_BUFFER_SIZE} \
    ")
ENDIF()

SET(ASM_COMMON_FLAGS " \
    -D__STARTUP_CLEAR_BSS \
    -mcpu=cortex-m33 \
    -Wall \
    -mthumb \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -mapcs \
    -std=gnu99 \
    ${FPU} \
")
SET(CMAKE_ASM_FLAGS_DEBUG " \
    ${CMAKE_ASM_FLAGS_DEBUG} \
    ${ASM_COMMON_FLAGS} \
    -DDEBUG \
")
SET(CMAKE_ASM_FLAGS_RELEASE " \
    ${CMAKE_ASM_FLAGS_RELEASE} \
    ${ASM_COMMON_FLAGS} \
    -DNDEBUG \
")


SET(C_COMMON_FLAGS " \
    -DCPU_MCXN947VDF_cm33_core0 \
    -DLV_CONF_INCLUDE_SIMPLE=1 \
    -DMCUXPRESSO_SDK \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSDK_OS_FREE_RTOS \
    -DPRINTF_FLOAT_ENABLE=1 \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DSD_ENABLED \
    -DUSE_RTOS=1 \
    -DDEMO_SDCARD \
    -DTF_LITE_STATIC_MEMORY \
    -DSDK_BOARD_ID=\"MCXN947\" \
    -DLCD_VIRTUAL_BUF_HEIGHT_DIVIDER=10 \
    -mcpu=cortex-m33 \
    -DBOARD_LCD_S035=1 \
    -Wall \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -mapcs \
    -std=gnu99 \
    ${AI_C_FLAGS} \
    ${NET_C_FLAGS} \
    ${SECURITY_C_FLAGS} \
    ${SHELL_C_FLAGS} \
    ${SYSTEM_VIEW_C_FLAGS} \
    ${MULTI_APP_C_FLAGS} \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_C_FLAGS_DEBUG " \
    ${CMAKE_C_FLAGS_DEBUG} \
    ${C_COMMON_FLAGS} \
    -DDEBUG \
")
SET(CMAKE_C_FLAGS_RELEASE " \
    ${CMAKE_C_FLAGS_RELEASE} \
    ${C_COMMON_FLAGS} \
    -DNDEBUG \
")


SET(CXX_COMMON_FLAGS " \
    -DCPU_MCXN947VDF_cm33_core0 \
    -DUSE_RTOS=1 \
    -DMCUXPRESSO_SDK \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DPRINTF_FLOAT_ENABLE=1 \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DSD_ENABLED \
    -DDEMO_SDCARD \
    -DTF_LITE_STATIC_MEMORY \
    -DLCD_VIRTUAL_BUF_HEIGHT_DIVIDER=10 \
    -mcpu=cortex-m33 \
    -DBOARD_LCD_S035=1 \
    -Wall \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -mapcs \
    -fno-rtti \
    -fno-exceptions \
    ${AI_C_FLAGS} \
    ${NET_C_FLAGS} \
    ${SECURITY_C_FLAGS} \
    ${SHELL_C_FLAGS} \
    ${SYSTEM_VIEW_C_FLAGS} \
    ${MULTI_APP_C_FLAGS} \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_DEBUG " \
    ${CMAKE_CXX_FLAGS_DEBUG} \
    ${CXX_COMMON_FLAGS} \
    -DDEBUG \
")
SET(CMAKE_CXX_FLAGS_RELEASE " \
    ${CMAKE_CXX_FLAGS_RELEASE} \
    ${CXX_COMMON_FLAGS} \
    -DNDEBUG \
")


SET(EXE_LINKER_COMMON_FLAGS " \
    -mcpu=cortex-m33 \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -u _printf_float \
    -mthumb \
    -mapcs \
    -Xlinker \
    --gc-sections \
    -Xlinker \
    -static \
    -Xlinker \
    -z \
    -Xlinker \
    muldefs \
    -Xlinker \
    -Map=output.map \
    -Wl,--print-memory-usage \
    -Xlinker \
    --defsym=__stack_size__=${STACK_SZ} \
    -Xlinker \
    --defsym=__heap_size__=${HEAP_SZ} \
    ${FPU} \
    ${SPECS} \
    -T\"${ProjDirPath}/MCXN947_cm33_core0_flash.ld\" -static \
")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG " \
    ${CMAKE_EXE_LINKER_FLAGS_DEBUG} \
    ${EXE_LINKER_COMMON_FLAGS} \
    -g \
")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_RELEASE} \
    ${EXE_LINKER_COMMON_FLAGS} \
")
