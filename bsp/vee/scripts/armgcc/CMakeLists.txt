# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.27.0)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Git)

project(npavee)

file (STRINGS "${ProjDirPath}/../../../nvee_version.txt" VEE_VERSION)

set(MCUX_SDK_PROJECT_NAME npavee.elf)
cmake_path(GET MCUX_SDK_PROJECT_NAME STEM MCUX_SDK_PROJECT_NAME_NO_EXT)

if (NOT DEFINED MicroEjRootDirPath)
    SET(MicroEjRootDirPath ${ProjDirPath}/../../../vee/)
endif()

if (NOT DEFINED SdkOverlayRootDirPath)
    SET(SdkOverlayRootDirPath ${ProjDirPath}/../../../sdk_overlay/)
endif()

if (NOT DEFINED VeeSdkRootDirPath)
    SET(VeeSdkRootDirPath ${ProjDirPath}/../../../mcux-sdk/core/)
endif()

if (NOT DEFINED SdkMiddlewareDirPath)
    SET(SdkMiddlewareDirPath ${ProjDirPath}/../../../mcux-sdk/middleware/)
endif()

if (NOT DEFINED RtosRootDirPath)
    SET(RtosRootDirPath ${SdkOverlayRootDirPath}/rtos/)
endif()

if (ENABLE_AI)
    set(ADDITIONAL_SRC "${MicroEjRootDirPath}/src/main/model_cifarnet_ops_npu.cpp")
else()
    set(ADDITIONAL_SRC "")
endif(ENABLE_AI)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

execute_process(COMMAND
  "${GIT_EXECUTABLE}" describe --always --abbrev=8 --dirty
  WORKING_DIRECTORY "${ProjDirPath}"
  OUTPUT_VARIABLE GIT_SHA_1
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

configure_file("${MicroEjRootDirPath}/src/main/tree_version.c.in" "${CMAKE_CURRENT_BINARY_DIR}/tree_version.c" @ONLY)

add_executable(${MCUX_SDK_PROJECT_NAME}
    "${MicroEjRootDirPath}/src/main/main.cpp"
    "${MicroEjRootDirPath}/src/main/main.h"
    "${MicroEjRootDirPath}/src/bsp/board.c"
    "${MicroEjRootDirPath}/src/bsp/board.h"
    "${MicroEjRootDirPath}/src/bsp/clock_config.c"
    "${MicroEjRootDirPath}/src/bsp/clock_config.h"
    "${MicroEjRootDirPath}/src/bsp/core_test_natives.c"
    "${MicroEjRootDirPath}/src/bsp/display_support.c"
    "${MicroEjRootDirPath}/src/bsp/display_support.h"
    "${MicroEjRootDirPath}/src/bsp/event_queue_test_natives.c"
    "${MicroEjRootDirPath}/src/bsp/gettimeofday_impl.c"
    "${MicroEjRootDirPath}/src/bsp/pin_mux.c"
    "${MicroEjRootDirPath}/src/bsp/pin_mux.h"
    "${MicroEjRootDirPath}/src/bsp/simple_gfx_app_imp.c"
    "${MicroEjRootDirPath}/src/bsp/FreeRTOSConfig.h"
    "${MicroEjRootDirPath}/src/bsp/lwipopts.h"
    "${MicroEjRootDirPath}/common/bsp/sdmmc/ffconf.h"
    "${MicroEjRootDirPath}/common/bsp/sdmmc/sdmmc_config.c"
    "${MicroEjRootDirPath}/common/bsp/sdmmc/sdmmc_config.h"
    "${CMAKE_CURRENT_BINARY_DIR}/tree_version.c"
    "${ADDITIONAL_SRC}"
)

IF(NOT DEFINED MICROEJ_DEST_OVERRIDE)
    SET( MICROEJ_DEST ${MicroEjRootDirPath})
    SET( MICROEJ_DEST_LIB "/lib")
ELSE()
    SET(MICROEJ_DEST ${CMAKE_BINARY_DIR})
    SET( MICROEJ_DEST_LIB "")
ENDIF()

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${MicroEjRootDirPath}/src/main
    ${MicroEjRootDirPath}/src/bsp
    ${MicroEjRootDirPath}/src/util/inc
    ${MicroEjRootDirPath}/
    ${MICROEJ_DEST}/inc
    ${MICROEJ_DEST}/inc/intern
    ${MicroEjRootDirPath}/common/bsp/sdmmc/
    ${MicroEjRootDirPath}/port/core/inc
    ${MicroEjRootDirPath}/port/cpuload/inc
    ${MicroEjRootDirPath}/port/ecom-network/inc
    ${MicroEjRootDirPath}/port/event-queue/inc
    ${MicroEjRootDirPath}/port/fs/inc
    ${MicroEjRootDirPath}/port/kf/inc
    ${MicroEjRootDirPath}/port/net/inc
    ${MicroEjRootDirPath}/port/ui/inc
    ${MicroEjRootDirPath}/port/util/inc
    ${SdkMiddlewareDirPath}/mbedtls/include
)

set_source_files_properties("${MicroEjRootDirPath}/src/bsp/FreeRTOSConfig.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_freertos-kernel_template")
set_source_files_properties("${MicroEjRootDirPath}/common/bsp/sdmmc/ffconf.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_fatfs_template_sd")
set_source_files_properties("${MicroEjRootDirPath}/common/bsp/sdmmc/sdmmc_config.c" PROPERTIES COMPONENT_CONFIG_FILE "middleware_sdmmc_usdhc_template")
set_source_files_properties("${MicroEjRootDirPath}/common/bsp/sdmmc/sdmmc_config.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_sdmmc_usdhc_template")

if (ENABLE_SEC)
    # mbedTLS Config
    target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PRIVATE MBEDTLS_CONFIG_FILE="microej_mbedtls_config.h")
endif(ENABLE_SEC)

set(CMAKE_MODULE_PATH
    ${MicroEjRootDirPath}/port/ai
    ${MicroEjRootDirPath}/port/core
    ${MicroEjRootDirPath}/port/cpuload
    ${MicroEjRootDirPath}/port/ecom-network
    ${MicroEjRootDirPath}/port/ecom-network-lwip
    ${MicroEjRootDirPath}/port/event-queue
    ${MicroEjRootDirPath}/port/fs
    ${MicroEjRootDirPath}/port/gpio
    ${MicroEjRootDirPath}/port/net
    ${MicroEjRootDirPath}/port/ui
    ${MicroEjRootDirPath}/port/util
    ${MicroEjRootDirPath}/port/security
    ${MicroEjRootDirPath}/port/ssl
    ${MicroEjRootDirPath}/src/util
    ${MicroEjRootDirPath}/common/bsp/sdmmc
    ${SdkOverlayRootDirPath}core/components/st7796s
    ${RtosRootDirPath}/freertos/freertos-kernel
    ${SdkMiddlewareDirPath}/fatfs
    ${SdkMiddlewareDirPath}/lwip
)

include(fsl_st7796s)
include(${VeeSdkRootDirPath}/devices/MCXN947/all_lib_device.cmake)
include(microej_core)
include(microej_cpuload)
include(microej_event_queue)
include(microej_fs)
include(microej_ui)
include(microej_util)
include(npa_util)
include(nxp_gpio)
include(utility_sdmmc)

if (ENABLE_SEC)
    include(microej_security)
    include(microej_ssl)
endif(ENABLE_SEC)

if (ENABLE_AI)
    include(nxpvee_ai)
endif(ENABLE_AI)

if (ENABLE_NET)
    include(microej_ecom-network)
    include(microej_ecom-network-lwip)
    include(microej_net)
endif(ENABLE_NET)

if (ENABLE_SYSTEM_VIEW)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
        ${MicroEjRootDirPath}/common/segger
        ${MicroEjRootDirPath}/port/trace
    )

    include(microej_trace)
    include(utility_segger)
endif(ENABLE_SYSTEM_VIEW)

if(ENABLE_MULTI_APP)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
        ${MicroEjRootDirPath}/port/kf
    )
    
    include(microej_kf)
endif(ENABLE_MULTI_APP)

SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${MICROEJ_DEST}${MICROEJ_DEST_LIB}/microejruntime.a)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${MICROEJ_DEST}${MICROEJ_DEST_LIB}/microejapp.o)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

add_custom_command(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Obinary ./${MCUX_SDK_PROJECT_NAME} ./${MCUX_SDK_PROJECT_NAME_NO_EXT}.bin)
add_custom_command(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Oihex ./${MCUX_SDK_PROJECT_NAME} ./${MCUX_SDK_PROJECT_NAME_NO_EXT}.hex)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ./${MCUX_SDK_PROJECT_NAME_NO_EXT}.bin)
set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ./${MCUX_SDK_PROJECT_NAME_NO_EXT}.hex)