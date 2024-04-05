/*
 * Copyright 2020-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
package com.microej.partial.support;

import ej.fp.Image;
import ej.fp.Widget.WidgetAttribute;
import ej.fp.Widget.WidgetDescription;
import ej.fp.widget.Display;
import ej.microui.display.LLUIPainter.MicroUIGraphicsContext;

/**
 * This {@link Display} widget supports the partial render policy: it can simulate a smaller buffer than display height.
 */
@WidgetDescription(attributes = { @WidgetAttribute(name = "label", isOptional = true), @WidgetAttribute(name = "x"),
		@WidgetAttribute(name = "y"), @WidgetAttribute(name = "width"), @WidgetAttribute(name = "height"),
		@WidgetAttribute(name = "displayWidth", isOptional = true),
		@WidgetAttribute(name = "displayHeight", isOptional = true),
		@WidgetAttribute(name = "initialColor", isOptional = true), @WidgetAttribute(name = "alpha", isOptional = true),
		@WidgetAttribute(name = "doubleBufferFeature", isOptional = true),
		@WidgetAttribute(name = "backlightFeature", isOptional = true),
		@WidgetAttribute(name = "filter", isOptional = true),
		@WidgetAttribute(name = "extensionClass", isOptional = true) })
public class PartialDisplay extends Display {

	private int flushX;
	private int flushY;
	private int flushWidth;
	private int flushHeight;

	@Override
	public void finalizeConfiguration() {
		super.finalizeConfiguration();

		this.flushWidth = getWidth();
		this.flushHeight = getHeight();
	}

	/**
	 * Sets the limits of the next flush operations.
	 *
	 * @param flushX
	 *            the x offset of the area to flush.
	 * @param flushY
	 *            the y offset of the area to flush.
	 * @param flushWidth
	 *            the width of the area to flush.
	 * @param flushHeight
	 *            the height of the area to flush.
	 */
	public void setFlushLimits(int flushX, int flushY, int flushWidth, int flushHeight) {
		this.flushX = flushX;
		this.flushY = flushY;
		this.flushWidth = flushWidth;
		this.flushHeight = flushHeight;
	}

	@Override
	public void flush(MicroUIGraphicsContext gc, Image backBuffer, int x, int y, int width, int height) {
		int flushWidth = Math.min(width, this.flushWidth);
		int flushHeight = Math.min(height, this.flushHeight);
		this.frameBuffer.drawImage(backBuffer, x, y, flushWidth, flushHeight, x + this.flushX, y + this.flushY,
				flushWidth, flushHeight);
		repaint();
	}
}
