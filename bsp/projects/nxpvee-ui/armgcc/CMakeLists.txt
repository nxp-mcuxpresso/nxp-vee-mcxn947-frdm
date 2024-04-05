# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)

# THE VERSION NUMBER
SET (MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION 2)
SET (MCUXPRESSO_CMAKE_FORMAT_MINOR_VERSION 0)

include(ide_overrides.cmake OPTIONAL)

if(CMAKE_SCRIPT_MODE_FILE)
  message("${MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION}")
  return()
endif()


# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})

find_package(Git)

project(nxpvee_ui)

file (STRINGS "${ProjDirPath}/../../../nvee_version.txt" VEE_VERSION)

set(MCUX_BUILD_TYPES debug release)

set(MCUX_SDK_PROJECT_NAME nxpvee_ui.elf)

if (NOT DEFINED MicroEjRootDirPath)
    SET(MicroEjRootDirPath ${ProjDirPath}/../../microej)
endif()

if (NOT DEFINED SdkOverlayRootDirPath)
    SET(SdkOverlayRootDirPath ${ProjDirPath}/../../../sdk_overlay/)
endif()

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../mcux-sdk/core)
endif()

if (NOT DEFINED SdkMiddlewareDirPath)
    SET(SdkMiddlewareDirPath ${ProjDirPath}/../../../mcux-sdk/middleware/)
endif()

if (NOT DEFINED RtosRootDirPath)
    SET(RtosRootDirPath ${ProjDirPath}/../../../mcux-sdk/rtos/)
endif()

set(KERNEL_ELF_NAME ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} PARENT_SCOPE)
set(KERNEL_BIN_NAME ${EXECUTABLE_OUTPUT_PATH}/nxpvee_ui.bin PARENT_SCOPE)

if (ENABLE_AI)
    set(ADDITIONAL_SRC "${ProjDirPath}/../app/model_cifarnet_ops_npu.cpp")
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

configure_file("${ProjDirPath}/../app/tree_version.c.in" "${CMAKE_CURRENT_BINARY_DIR}/tree_version.c" @ONLY)

add_executable(${MCUX_SDK_PROJECT_NAME}
"${ProjDirPath}/../config/inc/FreeRTOSConfig.h"
"${ProjDirPath}/../config/inc/lwipopts.h"
"${ProjDirPath}/../bsp/pin_mux.c"
"${ProjDirPath}/../bsp/pin_mux.h"
"${ProjDirPath}/../app/main.cpp"
"${ProjDirPath}/../bsp/board.c"
"${ProjDirPath}/../bsp/board.h"
"${ProjDirPath}/../bsp/clock_config.c"
"${ProjDirPath}/../bsp/clock_config.h"
"${ProjDirPath}/../../common/bsp/sdmmc/ffconf.h"
"${ProjDirPath}/../../common/bsp/sdmmc/sdmmc_config.c"
"${ProjDirPath}/../../common/bsp/sdmmc/sdmmc_config.h"
"${ProjDirPath}/../bsp/display_support.h"
"${ProjDirPath}/../bsp/display_support.c"
"${ProjDirPath}/../bsp/simple_gfx_app_imp.c"
"${CMAKE_CURRENT_BINARY_DIR}/tree_version.c"
"${ADDITIONAL_SRC}"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../bsp
    ${ProjDirPath}/../config/inc
    ${MicroEjRootDirPath}/
    ${MicroEjRootDirPath}/common/bsp/sdmmc/
    ${MicroEjRootDirPath}/core/inc
    ${MicroEjRootDirPath}/ecom-network/inc
    ${MicroEjRootDirPath}/fs/inc
    ${MicroEjRootDirPath}/net/inc
    ${MicroEjRootDirPath}/platform/inc
    ${MicroEjRootDirPath}/ui/inc
    ${MicroEjRootDirPath}/util/inc
    ${SdkMiddlewareDirPath}/mbedtls/include
)

set_source_files_properties("${ProjDirPath}/../config/inc/FreeRTOSConfig.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_freertos-kernel_template")
set_source_files_properties("${ProjDirPath}/../../common/bsp/sdmmc/ffconf.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_fatfs_template_sd")
set_source_files_properties("${ProjDirPath}/../../common/bsp/sdmmc/sdmmc_config.c" PROPERTIES COMPONENT_CONFIG_FILE "middleware_sdmmc_usdhc_template")
set_source_files_properties("${ProjDirPath}/../../common/bsp/sdmmc/sdmmc_config.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_sdmmc_usdhc_template")


if (ENABLE_SEC)
# mbedTLS Config
target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="microej_mbedtls_config.h")
endif(ENABLE_SEC)

set(CMAKE_MODULE_PATH
    ${MicroEjRootDirPath}/ai
    ${MicroEjRootDirPath}/core
    ${MicroEjRootDirPath}/gpio
    ${MicroEjRootDirPath}/fs
    ${MicroEjRootDirPath}/ecom-network
    ${MicroEjRootDirPath}/ecom-network-lwip
    ${MicroEjRootDirPath}/net
    ${MicroEjRootDirPath}/util
    ${MicroEjRootDirPath}/ui
    ${MicroEjRootDirPath}/security
    ${MicroEjRootDirPath}/ssl
    ${ProjDirPath}/../../common/bsp/sdmmc
    ${SdkMiddlewareDirPath}/fatfs
    ${SdkMiddlewareDirPath}/lwip
)

include(${SdkRootDirPath}/devices/MCXN947/all_lib_device.cmake)
include(microej_core)
include(microej_fs)
include(microej_util)
include(utility_sdmmc)
include(microej_ui)

if (ENABLE_SEC)
include(microej_security)
include(microej_ssl)
endif(ENABLE_SEC)

if (ENABLE_AI)
include(nxpvee_ai)
endif(ENABLE_AI)
include(nxp_gpio)

if (ENABLE_NET)
include(microej_ecom-network)
include(microej_ecom-network-lwip)
include(microej_net)
endif(ENABLE_NET)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")
ENDIF()

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${MicroEjRootDirPath}/platform/lib/microejruntime.a)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${MicroEjRootDirPath}/platform/lib/microejapp.o)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/nxpvee_ui.bin)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map;${EXECUTABLE_OUTPUT_PATH}/nxpvee_ui.bin")

