/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef DISPLAY_CONFIGURATION_H
#define DISPLAY_CONFIGURATION_H

/* structure that contains the region to update, unit: px */
typedef struct {
	int32_t x_offset;
	int32_t y_offset;
	uint32_t width;
	uint32_t height;
} DISPLAY_CONFIGURATION_flush_region;

#endif
