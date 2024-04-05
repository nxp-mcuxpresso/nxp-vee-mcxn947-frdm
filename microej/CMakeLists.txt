cmake_minimum_required(VERSION 3.2)

project(microej_comp NONE)

SET(PROJDIRPATH ${CMAKE_CURRENT_SOURCE_DIR})

list(APPEND CMAKE_MODULE_PATH "${PROJDIRPATH}/CMake")

add_library(microej_java dummy.c)

add_dependencies(microej_java FP CONFIGURATION APPS)

SET(FP ${PROJDIRPATH}/front-panel)
SET(CONF ${PROJDIRPATH}/vee-port-configuration)
SET(APP ${PROJDIRPATH}/apps)
SET(PLAT ${PROJDIRPATH}/MCXN947-frdm_platform-CM4hardfp_GCC48-1.0.0/source)

SET(ECLIPSE_HOME $ENV{ECLIPSE_HOME_VAR})
SET(MODULE_REPOSITORY_SETTINGS_FILE $ENV{MODULE_REPOSITORY_SETTINGS_FILE_VAR})
SET(MICROEJ_BUILDKIT_PATH $ENV{MICROEJ_BUILDKIT_PATH_VAR})


if (NOT DEFINED JUSAGE)
SET(JUSAGE "eval")
endif()
message("USAGE: ${JUSAGE}")

if (NOT DEFINED VERB)
SET(QUIET "--quiet")
else()
SET(QUIET "")
endif()

if (NOT DEFINED JMAIN)
SET(JMAIN "com.nxp.simpleGFX.SimpleGFX")
endif()
message("MAIN: ${JMAIN}")


add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/fp.guard" WORKING_DIRECTORY "${FP}"
                   COMMAND "${MICROEJ_BUILDKIT_PATH}/bin/mmm" "publish" ${QUIET}
                   "-Declipse.home=${ECLIPSE_HOME}"
                   "-Dcom.microej.platformbuilder.architecture.usage=${JUSAGE}"
                   "--module-repository-settings-file=${MODULE_REPOSITORY_SETTINGS_FILE}"
                   COMMAND "touch" "${CMAKE_CURRENT_BINARY_DIR}/fp.guard")

add_custom_target(FP DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/fp.guard")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/configuration.guard" WORKING_DIRECTORY "${CONF}"
                   COMMAND "${MICROEJ_BUILDKIT_PATH}/bin/mmm" "publish" ${QUIET}
                   "-Declipse.home=${ECLIPSE_HOME}"
                   "-Dcom.microej.platformbuilder.architecture.usage=${JUSAGE}"
                   "--module-repository-settings-file=${MODULE_REPOSITORY_SETTINGS_FILE}"
                   DEPENDS  ${CONF}/module.ivy
                   COMMAND "touch" "${CMAKE_CURRENT_BINARY_DIR}/configuration.guard")
add_custom_target(CONFIGURATION DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/configuration.guard")
add_dependencies(CONFIGURATION FP)

file(GLOB_RECURSE JAVA_SRCS ${APP}/*.java)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/apps.guard" WORKING_DIRECTORY "${APP}"
                   COMMAND "${MICROEJ_BUILDKIT_PATH}/bin/mmm" "publish" ${QUIET}
                   "-Declipse.home=${ECLIPSE_HOME}"
                   "--module-repository-settings-file=${MODULE_REPOSITORY_SETTINGS_FILE}"
                   "-Dcom.microej.platformbuilder.architecture.usage=${JUSAGE}"
                   "-Dplatform-loader.target.platform.dir=${PLAT}"
                   "-Dplatform-launcher.platform.dir=${PLAT}"
                   "-Dapplication.main.class=${JMAIN}"
                   DEPENDS  ${JAVA_SRCS} ${APP}/module.ivy
                   COMMAND "touch" "${CMAKE_CURRENT_BINARY_DIR}/apps.guard")
add_custom_target(APPS DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/apps.guard")
add_dependencies(APPS CONFIGURATION)

set_property(TARGET microej_java APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${FP}/fp/target~)
set_property(TARGET microej_java APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${APP}/target~)
set_property(TARGET microej_java APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${CONF}/target~)
set_property(TARGET microej_java APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${PROJDIRPATH}/../bsp/projects/microej/platform/)
