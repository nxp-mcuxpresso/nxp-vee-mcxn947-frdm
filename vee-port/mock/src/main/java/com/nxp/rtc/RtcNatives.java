/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

package com.nxp.rtc;

import java.time.Instant;

/**
 *
 */
public class RtcNatives {
	public static int getEpoch() {
		long now = Instant.now().toEpochMilli() / 1000;
		return (int)now;
	}
}
