include_guard()
message("microej/core component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
${CMAKE_CURRENT_LIST_DIR}/src/LLDEVICE_impl.c
${CMAKE_CURRENT_LIST_DIR}/src/fault_handlers.c
${CMAKE_CURRENT_LIST_DIR}/src/hooks_FreeRTOS.c
${CMAKE_CURRENT_LIST_DIR}/src/LLBSP_NXP.c
${CMAKE_CURRENT_LIST_DIR}/src/LLMJVM_FreeRTOS.c
${CMAKE_CURRENT_LIST_DIR}/src/microej_main.c
${CMAKE_CURRENT_LIST_DIR}/src/microej_time_freertos.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE    ${CMAKE_CURRENT_LIST_DIR}/inc)
