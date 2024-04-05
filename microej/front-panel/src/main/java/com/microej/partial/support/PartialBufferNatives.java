/**
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
package com.microej.partial.support;

import ej.fp.Device;

/**
 * Implements the natives of the partial buffer support library.
 */
public class PartialBufferNatives {

	/**
	 * Simulates a smaller buffer than display buffer. This constant should match
	 * with the constant BACK_BUFFER_HEIGHT of the BSP.
	 */
	private static final int BUFFER_HEIGHT = 32;

	private PartialBufferNatives() {
		// cannot instantiate it
	}

	/**
	 * Returns the platform the rendering buffer (back buffer) maximum height (in
	 * pixels).
	 * <p>
	 * The policy will ensure to not draw outside this limit.
	 *
	 * @return the platform rendering buffer height.
	 */
	public static int getBufferHeight() {
		return BUFFER_HEIGHT;
	}

	/**
	 * Sets the offset the platform has to use when will perform the next
	 * <code>Display.flush()</code>.
	 * <p>
	 * This offset indicates where flushing the rendering buffer (back buffer) in
	 * the frame buffer.
	 *
	 * @param xOffset
	 *            the offset the platform has to apply at next flush.
	 * @param yOffset
	 *            the offset the platform has to apply at next flush.
	 * @param width
	 *            the width of the widget to be flushed.
	 * @param height
	 *            the height of the widget to be flushed.
	 */
	public static void setFlushLimits(int xOffset, int yOffset, int width, int height) {
		PartialDisplay display = Device.getDevice().getWidget(PartialDisplay.class);
		if (display != null) {
			display.setFlushLimits(xOffset, yOffset, width, height);
		}
	}
}
