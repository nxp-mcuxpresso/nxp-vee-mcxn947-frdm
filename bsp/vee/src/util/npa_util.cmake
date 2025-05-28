# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

include_guard()
message("NXP PA util component is included.")

set(SRC_LIST "")

if (ENABLE_SHELL)
set(SRC_LIST ${SRC_LIST} "${CMAKE_CURRENT_LIST_DIR}/src/shell.c")
endif (ENABLE_SHELL)

set(SRC_LIST ${SRC_LIST} "${CMAKE_CURRENT_LIST_DIR}/src/rtc_natives.c")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
${SRC_LIST}
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/inc
)
