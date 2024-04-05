/**
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.simpleGFX;

import ej.microui.display.Display;
import ej.microui.display.GraphicsContext;
import ej.microui.display.Painter;
import ej.mwt.Widget;
import ej.mwt.animation.Animation;
import ej.mwt.util.Size;

/**
 *
 */
public class SimpleGFXWidget extends Widget implements Animation {

	final static int NXP_ORANGE = 0xF9B500;
	final static int NXP_BLUE = 0x0EAFE0;
	final static int NXP_GREEN = 0x69CA00;
	private final int widget_size;

	private static int grey = 0xFFFFFF;

	private static boolean decrease_grey = true;

	private final int reclen = 50;
	private final int recheight = 30;

	private int orange_x;
	private int orange_y;

	private int blue_x;
	private int blue_y;

	private int green_x;
	private int green_y;

	private int current_animation_time;

	@Override
	protected void onShown() {
		// start animation
		getDesktop().getAnimator().startAnimation(this);
	}

	/**
	 * Public constructor.
	 */
	public SimpleGFXWidget() {
		this.current_animation_time = 0;
		this.widget_size = Display.getDisplay().getWidth() < Display.getDisplay().getHeight()
				? Display.getDisplay().getWidth()
				: Display.getDisplay().getHeight();
	}

	@Override
	protected void computeContentOptimalSize(Size size) {
		// Do nothing
	}

	@Override
	public boolean tick(long platformTimeMillis) {
		// update widget state
		this.current_animation_time++;

		// update color of the circle
		if (decrease_grey) {
			grey -= 0x010101;
			if (grey == 0x808080) {
				decrease_grey = false;
			}
		} else {
			grey += 0x010101;
			if (grey == 0xffffff) {
				decrease_grey = true;
			}
		}

		// Update position of the rectangles
		this.orange_x = SimpleGFXNatives.get_orange_x(this.current_animation_time, this.reclen, this.recheight, this.widget_size);
		this.orange_y = SimpleGFXNatives.get_orange_y(this.current_animation_time, this.reclen, this.recheight, this.widget_size);

		this.blue_x = SimpleGFXNatives.get_blue_x(this.current_animation_time, this.reclen, this.recheight, this.widget_size);
		this.blue_y = SimpleGFXNatives.get_blue_y(this.current_animation_time, this.reclen, this.recheight, this.widget_size);

		this.green_x = SimpleGFXNatives.get_green_x(this.current_animation_time, this.reclen, this.recheight, this.widget_size);
		this.green_y = SimpleGFXNatives.get_green_y(this.current_animation_time, this.reclen, this.recheight, this.widget_size);

		// request new render
		this.getParent().requestRender();

		// return whether to continue or to stop the animation
		return true;
	}

	@Override
	protected void renderContent(GraphicsContext g, int contentWidth, int contentHeight) {
		g.setColor(grey);
		Painter.fillCircle(g, 0, 0, this.widget_size);
		// int centre = this.DISPLAY_SIZE / 2;
		// double sine_orange = 30.0 * Math.cos(t * Math.PI / 45.0);
		// double sine_blue = 75.0 * Math.sin(t * Math.PI / 45.0);
		// draw_rect(centre - reclen - reclen / 2 - 30 - (int) sine_orange, centre - recheight / 2, reclen, recheight,
		// this.NXP_ORANGE);
		draw_rect(g, this.orange_x, this.orange_y, this.reclen, this.recheight, NXP_ORANGE);
		// draw_rect(centre - reclen / 2, centre - recheight / 2 + (int) sine_blue, reclen, recheight, this.NXP_BLUE);
		draw_rect(g, this.blue_x, this.blue_y, this.reclen, this.recheight, NXP_BLUE);
		// draw_rect(centre + reclen - reclen / 2 + 30 + (int) sine_orange, centre - recheight / 2, reclen, recheight,
		// this.NXP_GREEN);
		draw_rect(g, this.green_x, this.green_y, this.reclen, this.recheight, NXP_GREEN);
	}

	/**
	 * Draw a simple rectangle
	 *
	 * @param g
	 *            the graphics context where to render the content of the widget.
	 * @param x
	 *            the x coordinate of the rectangle.
	 * @param y
	 *            the Y coordinate of the rectangle.
	 * @param width
	 *            the width of the rectangle.
	 * @param height
	 *            the height of the rectangle.
	 * @param color
	 *            the color of the rectangle.
	 */
	private void draw_rect(GraphicsContext g, int x, int y, int width, int height, int color) {
		g.setColor(color);
		Painter.fillRectangle(g, x, y, width, height);
	}
}
