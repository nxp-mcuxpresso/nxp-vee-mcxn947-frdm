cmake_minimum_required(VERSION 3.2)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE debug)
endif()

if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "bsp/mcux-sdk/core/tools/cmake_toolchain_files/armgcc.cmake")
endif()

project(947Vee)


SET(PROJDIRPATH ${CMAKE_CURRENT_SOURCE_DIR})
SET(JC ${CMAKE_CURRENT_SOURCE_DIR}/microej/CMake/jcc.py)

option(VALIDATION "with(out) VALIDATION option")
if(VALIDATION)
    add_definitions(-DVALIDATION)
endif(VALIDATION)


set(ZEPHYR_BASE ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "")
set(CACHED_BOARD mcxn9xxbrk_cm33_cpu0  CACHE STRING "")
set(ZEPHYR_RUNNERS_YAML ${CMAKE_CURRENT_SOURCE_DIR}/build/runners.yaml  CACHE STRING "")
set(CONFIG_FLASH_BASE_ADDRESS 0x10000000 CACHE STRING "")
set(CMAKE_GDB gdb-multiarch CACHE STRING "")
set(BOARD_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "")


add_custom_target(runners_yaml_props_target)
add_custom_target(config_props_target)


set(ignoreMe "${WEST_PYTHON}")

include(cmake/modules/extensions.cmake OPTIONAL)
include(cmake/board/board.cmake OPTIONAL)


add_subdirectory(microej)
add_subdirectory(bsp/projects/nxpvee-ui/armgcc)
add_subdirectory(cmake/flash)

add_dependencies(nxpvee_ui.elf APPS)

function(config_append content)
  # Append ${content}\n to a target property which is later evaluated as a
  # generator expression when writing the flash runner yaml file.
  # We define this function here to have access to the `flash` target.

  set_property(
    TARGET         config_props_target
    APPEND_STRING
    PROPERTY       config_contents
    "${content}\n"
    )
endfunction()


function(create_config)
  set(config_file "${PROJECT_BINARY_DIR}/.config")

config_append ("CONFIG_FLASH_BASE_ADDRESS=${CONFIG_FLASH_BASE_ADDRESS}")

  # Write the final contents and set its location in the cache.
  file(GENERATE OUTPUT "${config_file}" CONTENT
    $<TARGET_PROPERTY:config_props_target,config_contents>)
endfunction()

create_config()
