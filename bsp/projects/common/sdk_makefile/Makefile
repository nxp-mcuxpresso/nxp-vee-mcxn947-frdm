#
# Copyright 2023 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#

COMMON_PATH=../../common/sdk_makefile

define del_file
	$(if $(filter $(OS),Windows_NT),$(if $(wildcard $(1)),cmd /c DEL /f /q $(subst /,\\,$(1)),),rm -f $(1))
endef

define del_dir
	$(if $(filter $(OS),Windows_NT),$(if $(wildcard $(1)),cmd /c RD /s /q $(subst /,\\,$(1)),),rm -fr $(1))
endef

ifeq ($(OS),Windows_NT)
JLINKEXE="$(JLINK_INSTALLATION_DIR)/Jlink.exe"
JLINK_GDB="$(COMMON_PATH)/debug/jlink_gdb.bat"
else
JLINKEXE=JLinkExe
JLINK_GDB="$(COMMON_PATH)"/debug/jlink_gdb.sh
endif

JLINK_SCRIPT="jlink_flash.script"

MAKE ?= make
OBJCOPY="$(ARMGCC_DIR)/bin/arm-none-eabi-objcopy"


ifeq ($(OS),Windows_NT)
GDB="$(ARMGCC_DIR)/bin/arm-none-eabi-gdb.exe"
NUM_PROC=
CLEAN_SCRIPT="./clean.bat"
else
GDB=gdb-multiarch
NUM_PROC=$(shell nproc)
CLEAN_SCRIPT="./clean.sh"
endif

ifeq ($(MAKE),make)
MAKE_OP=-j$(NUM_PROC)
endif

BUILD_DIR=../armgcc/$(FLAVOUR)

all: ../armgcc/$(FLAVOUR) remake

ifeq ($(OS),Windows_NT)
BUILD_SCRIPT="./build_$(FLAVOUR).bat"
CMSIS_GDB_TEMP_FOLDER="$(TEMP)"/gdb.cmd
PROBE=
else
BUILD_SCRIPT=./build_$(FLAVOUR).sh
CMSIS_GDB_TEMP_FOLDER=/tmp/gdb.cmd
PROBE=--probeserial $(shell echo probelist | redlinkserv --commandline  | grep 'Serial Number' | cut -d\=  -f2)
endif

../armgcc/$(FLAVOUR):
	cd ../armgcc/ && $(BUILD_SCRIPT) $(MAKE) $(CMAKE_OPTS)

remake:
	$(MAKE) $(MAKE_OP) -C ../armgcc/
	$(OBJCOPY) -Obinary "$(BUILD_DIR)/$(TARGET).elf" "$(BUILD_DIR)/$(TARGET).bin"
	$(OBJCOPY) -Oihex "$(BUILD_DIR)/$(TARGET).elf" "$(BUILD_DIR)/$(TARGET).hex"

clean:
ifneq ("$(wildcard $(../armgcc/Makefile))","")
	$(MAKE) -C ../armgcc/ clean
endif
	$(call del_dir,$(BUILD_DIR))

distclean:
	cd ../armgcc/ && $(CLEAN_SCRIPT)

flash: ../armgcc/$(FLAVOUR) remake
	$(call del_file,$(JLINK_SCRIPT))
	echo r > $(JLINK_SCRIPT)
	echo loadbin "$(BUILD_DIR)/$(TARGET).bin" $(ADDRESS) >> $(JLINK_SCRIPT)
	echo r >> $(JLINK_SCRIPT)
	echo g >> $(JLINK_SCRIPT)
	echo qc >> $(JLINK_SCRIPT)
	$(JLINKEXE) -NoGui 1 -device $(DEVICE) -If SWD -Speed 10000 -CommanderScript $(JLINK_SCRIPT)
	$(call del_file,$(JLINK_SCRIPT))

flash_cmsisdap: ../armgcc/$(FLAVOUR) remake
	crt_emu_cm_redlink --flash-load-exec $(BUILD_DIR)/$(TARGET).elf \
    -p MCXN947 --bootromstall 0x50000040 $(PROBE) \
    -x $(COMMON_PATH)/cmsisdap_support \
    -CoreIndex=0 --flash-driver= \
    --PreconnectScript LS_preconnect_MCXN9XX.scp \
    --flash-hashing

gdb_cmsisdap: ../armgcc/$(FLAVOUR) remake
	sed "s+FILE_VALUE+$(BUILD_DIR)/$(TARGET).elf+;s+PROBESERIAL+$(PROBE)+" $(COMMON_PATH)/cmsisdap_support/gdb.commands > $(CMSIS_GDB_TEMP_FOLDER)
	$(GDB) --command=$(CMSIS_GDB_TEMP_FOLDER)

gdb: ../armgcc/$(FLAVOUR) remake
	$(JLINK_GDB) $(DEVICE)
	$(GDB) --command=$(COMMON_PATH)/debug/jlink.gdb $(BUILD_DIR)/$(TARGET).elf

.PHONY : all remake clean dist_clean gdb
