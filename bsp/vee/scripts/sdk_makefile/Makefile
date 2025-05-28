#
# Copyright 2023-2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
TARGET=npavee
DEVICE=MCXN947
LS_DEVICE=MCXN947:MCX-N9XX-BRK
ADDRESS=0x10000000
FLAVOUR=debug

ifeq ($(strip $(RELEASE)), 1)
	FLAVOUR=release
endif

BUILD_DIR=../armgcc/$(FLAVOUR)

include ../../common/sdk_makefile/Makefile