RD /s /Q CMakeFiles
DEL /s /Q /F Makefile cmake_install.cmake CMakeCache.txt
DEL /s /Q /F tree_version.c
IF EXIST debug RD /s /Q debug
IF EXIST release RD /s /Q release
