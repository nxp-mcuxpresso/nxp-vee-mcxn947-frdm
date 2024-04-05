#
# Copyright 2023 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
TARGET=nxpvee_ui
DEVICE=MCXN947
ADDRESS=0x10000000
FLAVOUR=debug

ifeq ($(strip $(RELEASE)),1)
	FLAVOUR=release
endif

include ../../common/sdk_makefile/Makefile
