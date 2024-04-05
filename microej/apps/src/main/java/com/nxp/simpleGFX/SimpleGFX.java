/**
 * Copyright 2022-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
package com.nxp.simpleGFX;

import com.microej.partial.support.PartialRenderPolicy;

import ej.bon.Constants;
import ej.microui.MicroUI;
import ej.microui.display.Display;
import ej.microui.event.Event;
import ej.microui.event.generator.Buttons;
import ej.microui.event.generator.Pointer;
import ej.mwt.Desktop;
import ej.mwt.render.DefaultRenderPolicy;
import ej.mwt.render.RenderPolicy;
import ej.util.Device;
import ej.widget.container.Canvas;

/**
 *
 */
public class SimpleGFX {

	private final SimpleGFXWidget simpleGFXWidget;
	private static SimpleGFX simpleGFX;

	/**
	 * Public constructor
	 */
	public SimpleGFX() {
		String id = new String(Device.getId());
		System.out.println("Board id: " + id);
		Display display = Display.getDisplay();

		int displayMinimumSize = Display.getDisplay().getWidth() < Display.getDisplay().getHeight()
				? Display.getDisplay().getWidth()
				: Display.getDisplay().getHeight();

		Desktop desktop = new Desktop() {
			@Override
			protected RenderPolicy createRenderPolicy() {
				if (Constants.getBoolean("com.microej.partial.support.isEnabled")) {
					System.out.println("Using PartialRenderPolicy");
					return new PartialRenderPolicy(this, false);
				} else {
					System.out.println("Using DefaultRenderPolicy");
					return new DefaultRenderPolicy(this);
				}
			}

			@Override
			public boolean handleEvent(int event) {
				int type = Event.getType(event);
				int data = Event.getData(event);

				if (type == Pointer.EVENT_TYPE) {
					int action = Buttons.getAction(event);
					if (action == Buttons.PRESSED) {
						Pointer pointer = (Pointer) Event.getGenerator(event);

						int x = pointer.getX();
						int y = pointer.getY();

						System.out.println("x = " + x + ", y = " + y);
					}
				}
				return true;
			}
		};

		Canvas container = new Canvas();

		this.simpleGFXWidget = new SimpleGFXWidget();

		this.simpleGFXWidget.setEnabled(true);

		container.addChild(this.simpleGFXWidget, 0, 0, displayMinimumSize, displayMinimumSize);
		desktop.setWidget(container);

		desktop.requestShow();
	}

	/**
	 * Simple main.
	 *
	 * @param args
	 *            command line arguments.
	 */
	public static void main(String[] args) {
		MicroUI.start();
		simpleGFX = new SimpleGFX();
	}

	public static SimpleGFXWidget getGFXWidget() {
		return simpleGFX.simpleGFXWidget;
	}

}
