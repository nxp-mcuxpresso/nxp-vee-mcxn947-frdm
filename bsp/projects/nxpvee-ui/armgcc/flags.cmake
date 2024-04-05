IF(NOT DEFINED FPU)  
    SET(FPU "-mfloat-abi=hard -mfpu=fpv5-sp-d16")  
ENDIF()  

IF(NOT DEFINED SPECS)  
    SET(SPECS "--specs=nano.specs --specs=nosys.specs")  
ENDIF()  

IF(NOT DEFINED DEBUG_CONSOLE_CONFIG)  
    SET(DEBUG_CONSOLE_CONFIG "-DSDK_DEBUGCONSOLE=1")  
ENDIF()  

IF(ENABLE_AI)
    SET(STACK_SZ 0x2000)
    SET(HEAP_SZ 0x2F800)
    SET(AI_C_FLAG "-DENABLE_AI")  
ELSE()
    SET(STACK_SZ 0x800)
    SET(HEAP_SZ 0x0400)
ENDIF()
IF(ENABLE_NET)
    SET(NET_C_FLAG "-DENABLE_NET")  
ENDIF()
IF(ENABLE_SEC)
    SET(SECURITY_C_FLAG " \
    -DENABLE_SEC \
    -DMBEDTLS_MCUX_ELS_PKC_API \
    -DMBEDTLS_MCUX_USE_PKC \
    -DMBEDTLS_MCUX_ELS_API \
    -DMBEDTLS_MCUX_USE_ELS \
    -DMCUXCL_FEATURE_CSSL_MEMORY_C_FALLBACK \
")
ENDIF()

SET(CMAKE_ASM_FLAGS_DEBUG " \
    ${CMAKE_ASM_FLAGS_DEBUG} \
    -DDEBUG \
    -D__STARTUP_CLEAR_BSS \
    -g \
    -mcpu=cortex-m33 \
    -Wall \
    -mthumb \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -std=gnu99 \
    ${FPU} \
")
SET(CMAKE_ASM_FLAGS_RELEASE " \
    ${CMAKE_ASM_FLAGS_RELEASE} \
    -DNDEBUG \
    -D__STARTUP_CLEAR_BSS \
    -mcpu=cortex-m33 \
    -Wall \
    -mthumb \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -std=gnu99 \
    ${FPU} \
")
SET(CMAKE_C_FLAGS_DEBUG " \
    ${CMAKE_C_FLAGS_DEBUG} \
    -DDEBUG \
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
    -g \
    -O0 \
    -mcpu=cortex-m33 \
    -DBOARD_LCD_S035=1 \
    -Wall \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -std=gnu99 \
    ${AI_C_FLAG} \
    ${NET_C_FLAG} \
    ${SECURITY_C_FLAG} \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_C_FLAGS_RELEASE " \
    ${CMAKE_C_FLAGS_RELEASE} \
    -DNDEBUG \
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
    -Os \
    -mcpu=cortex-m33 \
    -DBOARD_LCD_S035=1 \
    -Wall \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -std=gnu99 \
    ${AI_C_FLAG} \
    ${NET_C_FLAG} \
    ${SECURITY_C_FLAG} \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_DEBUG " \
    ${CMAKE_CXX_FLAGS_DEBUG} \
    -DDEBUG \
    -DCPU_MCXN947VDF_cm33_core0 \
    -DUSE_RTOS=1 \
    -DMCUXPRESSO_SDK \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DPRINTF_FLOAT_ENABLE=1 \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DSD_ENABLED \
    -DDEMO_SDCARD \
    -DTF_LITE_STATIC_MEMORY \
    -g \
    -O0 \
    -mcpu=cortex-m33 \
    -DBOARD_LCD_S035=1 \
    -Wall \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -fno-rtti \
    -fno-exceptions \
    ${AI_C_FLAG} \
    ${NET_C_FLAG} \
    ${SECURITY_C_FLAG} \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_RELEASE " \
    ${CMAKE_CXX_FLAGS_RELEASE} \
    -DNDEBUG \
    -DCPU_MCXN947VDF_cm33_core0 \
    -DUSE_RTOS=1 \
    -DMCUXPRESSO_SDK \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DPRINTF_FLOAT_ENABLE=1 \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DSD_ENABLED \
    -DDEMO_SDCARD \
    -DTF_LITE_STATIC_MEMORY \
    -Os \
    -mcpu=cortex-m33 \
    -DBOARD_LCD_S035=1 \
    -Wall \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -fno-rtti \
    -fno-exceptions \
    ${AI_C_FLAG} \
    ${NET_C_FLAG} \
    ${SECURITY_C_FLAG} \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG " \
    ${CMAKE_EXE_LINKER_FLAGS_DEBUG} \
    -g \
    -mcpu=cortex-m33 \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
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
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_RELEASE} \
    -mcpu=cortex-m33 \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
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
