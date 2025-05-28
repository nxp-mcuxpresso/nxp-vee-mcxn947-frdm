/*
 * Java
 *
 * Copyright 2013-2024 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */
package com.microej.core.tests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.io.InputStream;
import java.io.OutputStream;
import java.io.Reader;
import java.io.Writer;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.NoSuchElementException;

import org.junit.BeforeClass;
import org.junit.Test;

import ej.bon.Immortals;
import ej.bon.Immutables;
import ej.bon.Util;

/**
 * MicroEJ Core Validation tests.
 */
public class MicroejCoreValidation {

	private static final String VERSION = "3.4.0";

	private static final String PROPERTY_PREFIX = "com.microej.core.tests.";
	private static final String OPTION_CLOCK_NB_SECONDS = "clock.seconds";
	private static final String OPTION_MONOTONIC_CHECK_NB_SECONDS = "monotonic.time.check.seconds";
	/**
	 * Option that specifies the maximum allowed value for the duration of a clock tick. The higher this value is the
	 * lower the allowed clock resolution will be.
	 */
	private static final String OPTION_MAX_ALLOWED_CLOCK_TICK_DURATION_MS = "max.allowed.clock.tick.duration.milliseconds";
	private static final int DEFAULT_MAX_ALLOWED_CLOCK_TICK_DURATION_MS = 20;
	/**
	 * Option that specifies whether the system time can be set on the platform being qualified.
	 */
	private static final String OPTION_CAN_SET_SYSTEM_TIME = "can.set.system.time";
	private static final boolean DEFAULT_CAN_SET_SYSTEM_TIME = true;

	private static final String INVALID_C_FUNCTION_MESSAGE = "C function not correctly implemented (check your libc configuration)";
	private static final String INCOHERENT_FPU_MESSAGE = "FPU option is not coherent between MicroEJ Core and BSP";

	private static Class<MicroejCoreValidation> THIS_CLASS = MicroejCoreValidation.class;

	// Round Robin constants
	private static final int NB_THREADS = 4;
	private static final int ROUND_ROBIN_TEST_DURATION = 10000;
	private static final int ROUND_ROBIN_MIN_COUNTER_REQUIRED = 500;
	private static final int ROUND_ROBIN_MAX_DELTA_PERCENTAGE_ALLOWED = 5;

	private static volatile boolean ROUND_ROBIN_IS_RUNNING;
	private static Object ROUND_ROBIN_LOCK = new Object();
	private static long[] ROUND_ROBIN_TASK_COUNTERS;

	// Set this fields volatile so we are sure accesses are not optimized
	volatile private static double double3 = 3d;
	volatile private static double double4 = 4d;
	volatile private static double doubleNaN = Double.NaN;
	volatile private static double doubleZero = 0.0;
	volatile private static double doubleNegativeZero = -0.0;
	volatile private static double doublePositiveInfinity = Double.POSITIVE_INFINITY;
	volatile private static double doubleNegativeInfinity = Double.NEGATIVE_INFINITY;
	volatile private static float float3 = 3f;
	volatile private static float float4 = 4f;
	volatile private static float floatNaN = Float.NaN;
	volatile private static float floatZero = 0.0f;
	volatile private static float floatNegativeZero = -0.0f;
	volatile private static float floatPositiveInfinity = Float.POSITIVE_INFINITY;
	volatile private static float floatNegativeInfinity = Float.NEGATIVE_INFINITY;
	volatile private static int intMin = Integer.MIN_VALUE;
	volatile private static int intNegativeOne = -1;
	volatile private static long longMin = Long.MIN_VALUE;
	volatile private static long longNegativeOne = -1l;

	// testParseDoubleStringHeap: tolerance value for float comparison
	private static final float FLOAT_COMPARISON_TOLERANCE_PERCENT = 0.01f;

	private static void printProduct() {
		final String sep = "*****************************************************************************************************";
		System.out.println(sep);
		System.out.println("*                                  MicroEJ Core Validation - " + VERSION
				+ "                                  *");
		System.out.println(sep);
		System.out.println(
				"* Copyright 2013-2024 MicroEJ Corp. All rights reserved.                                            *");
		System.out.println(
				"* Use of this source code is governed by a BSD-style license that can be found with this software.  *");
		System.out.println(sep);
		System.out.println();
	}

	private static int getOptionAsInt(String optionName, int defaultValue, String unit) {
		String propertyName = PROPERTY_PREFIX + optionName;
		String valueStr = System.getProperty(propertyName);
		int value;
		if (valueStr == null) {
			value = defaultValue;
			System.out.println("Property '" + propertyName + "' is not set (default to '" + value + "' " + unit
					+ (value > 1 ? "s" : "") + ")");
		} else {
			try {
				value = Integer.parseInt(valueStr);
				System.out.println(
						"Property '" + propertyName + "' is set to '" + value + "' " + unit + (value > 1 ? "s" : ""));
			} catch (NumberFormatException e) {
				value = defaultValue;
				System.out.println("Property '" + propertyName + "' is invalid (set to '" + valueStr + "', default to '"
						+ value + "' " + unit + (value > 1 ? "s" : "") + ")");
			}
		}
		return value;
	}

	private static boolean getOptionAsBool(final String optionName, boolean defaultValue) {
		final String propertyName = PROPERTY_PREFIX + optionName;
		String valueStr = System.getProperty(propertyName);
		boolean value;

		if (valueStr == null) {
			value = defaultValue;
			System.out.println("Property '" + propertyName + "' is not set (default to '" + value + "')");
		} else {
			value = Boolean.parseBoolean(valueStr);
			System.out.println("Property '" + propertyName + "' is set to '" + value + "'");
		}

		return value;
	}

	private static void tryToSynchronizeOn(List<Object[]> objects, boolean mustFail) {
		MonitorKeeper.errorCount = 0;
		Thread[] threads = new Thread[MonitorKeeper.THREAD_COUNT];

		try {
			for (int i = 0; i < threads.length; i++) {
				threads[i] = new Thread(new MonitorKeeper(objects.get(i)));
			}

			for (Thread thread : threads) {
				thread.start();
			}

			for (Thread thread : threads) {
				try {
					thread.join();
				} catch (InterruptedException e) {
					// Nothing to do here.
				}
			}

			boolean success;
			if (mustFail) {
				success = MonitorKeeper.errorCount > 0;
			} else {
				success = MonitorKeeper.errorCount == 0;
			}
			assertTrue("Too many synchronized monitors.", success);
		} catch (IndexOutOfBoundsException ioobe) {
			fail("No objects to synchronize on, aborting.");
		}
	}

	private static List<Object[]> objectsFromRam() {
		List<Object[]> objects = new ArrayList<>();

		for (int i = 0; i < MonitorKeeper.THREAD_COUNT; i++) {
			Object[] objects2 = new Object[MonitorKeeper.MONITOR_PER_THREAD_COUNT];
			for (int j = 0; j < objects2.length; j++) {
				objects2[j] = new Object();
			}
			objects.add(objects2);
		}

		return objects;
	}

	private static List<Object[]> objectsFromImmortals() {
		List<Object[]> objects = new ArrayList<>();

		for (int i = 0; i < MonitorKeeper.THREAD_COUNT; i++) {
			Object[] objects2 = new Object[MonitorKeeper.MONITOR_PER_THREAD_COUNT];
			for (int j = 0; j < objects2.length; j++) {
				Object object = new Object();
				Immortals.setImmortal(object);
				objects2[j] = object;
			}
			objects.add(objects2);
		}

		return objects;
	}

	private static List<Object[]> objectsFromImmutables() {
		List<Object[]> objects = new ArrayList<>();
		int objectID = 0;

		try {
			for (int i = 0; i < MonitorKeeper.THREAD_COUNT; i++) {
				Object[] objects2 = new Object[MonitorKeeper.MONITOR_PER_THREAD_COUNT];
				for (int j = 0; j < objects2.length; j++) {
					Object object = Immutables.get("array" + objectID++);
					objects2[j] = object;
				}
				objects.add(objects2);
			}
		} catch (NoSuchElementException nsee) {
			System.out.println("Can't find the requested object in the immutables, check your launch configuration.");
		}

		return objects;
	}

	private static List<Object[]> stringsFromImmutables() {
		List<Object[]> objects = new ArrayList<>();

		objects.add(new String[] { "0", "1", "2", "3", "4", "5", "6" });
		objects.add(new String[] { "7", "8", "9", "10", "11", "12", "13" });
		objects.add(new String[] { "14", "15", "16", "17", "18", "19", "20" });
		objects.add(new String[] { "21", "22", "23", "24", "25", "26", "27" });
		objects.add(new String[] { "28", "29", "30", "31", "32", "33", "34" });

		return objects;
	}

	private static List<Object[]> classes() {
		List<Object[]> objects = new ArrayList<>();

		objects.add(new Class[] { Integer.class, Boolean.class, Long.class, Float.class, Double.class, Thread.class,
				Object.class });
		objects.add(new Class[] { Byte.class, Character.class, Calendar.class, ArrayList.class, List.class, Class.class,
				Exception.class });
		objects.add(new Class[] { InputStream.class, OutputStream.class, Map.class, HashMap.class, Hashtable.class,
				Reader.class, Writer.class });
		objects.add(new Class[] { Number.class, Throwable.class, String.class, Short.class, Enum.class, Runtime.class,
				Package.class });
		objects.add(new Class[] { System.class, Math.class, StringBuilder.class, StringBuffer.class, Runnable.class,
				Iterable.class, Iterator.class });

		return objects;
	}

	private static float testFPUJava(float a, float b) {
		return a * b;
	}

	private static double testFPUJava(double a, double b) {
		return a * b;
	}

	/**
	 * @throws java.lang.Exception
	 *             If an error occurred.
	 */
	@BeforeClass
	public static void setUpBeforeClass() throws Exception {
		printProduct();
	}

	/**
	 * Tests the <code>LLMJVM_IMPL_getCurrentTime</code> implementation.
	 */
	@Test
	public void testVisibleClock() {
		System.out.println("-> Check visible clock (LLMJVM_IMPL_getCurrentTime validation)...");
		int defaultNbSeconds = 10;
		int nbSeconds = getOptionAsInt(OPTION_CLOCK_NB_SECONDS, defaultNbSeconds, "second");

		// Check if a message is printed every seconds in terminal:
		long timeStart = System.currentTimeMillis();
		int seconds = 0;
		long nbMilliSeconds = nbSeconds * 1000;
		while (true) {
			long time = System.currentTimeMillis();
			long delta = time - timeStart;
			int newSecond = (int) (delta / 1000);
			if (newSecond > seconds) {
				System.out.println(newSecond);
				seconds = newSecond;
			}

			if (delta > nbMilliSeconds) {
				break; // end of test
			}
		}
	}

	/**
	 * Tests <code>LLMJVM_IMPL_scheduleRequest</code>, <code>LLMJVM_IMPL_wakeupVM</code>,
	 * <code>LLMJVM_IMPL_getCurrentTime</code>, and <code>LLMJVM_IMPL_getTimeNanos</code> implementations.
	 */
	@Test
	public void testTime() {
		System.out.println(
				"-> Check schedule request and wakeup (LLMJVM_IMPL_scheduleRequest, LLMJVM_IMPL_wakeupVM, LLMJVM_IMPL_getCurrentTime, and LLMJVM_IMPL_getTimeNanos validation)...");
		final long precisionLimit = getOptionAsInt(OPTION_MAX_ALLOWED_CLOCK_TICK_DURATION_MS,
				DEFAULT_MAX_ALLOWED_CLOCK_TICK_DURATION_MS, "milliseconds");
		final long precisionLimitNano = precisionLimit * 1000000l;
		long delay = 5 * 1000;
		long delayNano = delay * 1000000l;
		System.out.println("Waiting for " + delay / 1000 + "s...");
		long timeBefore = Util.platformTimeMillis();
		long nanoTimeBefore = Util.platformTimeNanos();
		try {
			Thread.sleep(delay);
		} catch (InterruptedException e) {
			throw new Error();
		}
		long timeAfter = Util.platformTimeMillis();
		long nanoTimeAfter = Util.platformTimeNanos();
		System.out.println("...done");

		long realDelay = timeAfter - timeBefore;
		assertTrue("ms realDelay>=delay", realDelay >= delay);
		long delta = realDelay - delay;
		assertTrue("ms delta(=" + delta + ")<=" + precisionLimit, delta <= precisionLimit);

		long realDelayNano = nanoTimeAfter - nanoTimeBefore;
		assertTrue("ns realDelay>=delay", realDelayNano >= delayNano);
		long deltaNano = realDelayNano - delayNano;
		assertTrue("ns delta(=" + deltaNano + ")<=" + precisionLimitNano, deltaNano <= precisionLimitNano);
	}

	/**
	 * Tests the <code>LLMJVM_IMPL_setApplicationTime</code> implementation.
	 */
	@Test
	public void testSetApplicationTime() {
		System.out.println(
				"-> Check application time modification (LLMJVM_IMPL_getCurrentTime and LLMJVM_IMPL_setApplicationTime validation)...");
		final long delay = 5 * 1000;
		final long toleranceTime = 100;
		final long timeOffset = 50_000;

		final boolean canSetSystemTime = getOptionAsBool(OPTION_CAN_SET_SYSTEM_TIME, DEFAULT_CAN_SET_SYSTEM_TIME);

		if (canSetSystemTime) {
			System.out.println("Set application time and wait for " + delay / 1000 + "s...");

			final long applicationTimeBefore = System.currentTimeMillis();
			final long monotonicTimeBefore = Util.platformTimeMillis();
			Util.setCurrentTimeMillis(applicationTimeBefore + timeOffset);

			try {
				Thread.sleep(delay);
			} catch (InterruptedException e) {
				throw new Error();
			}

			final long applicationTimeAfter = System.currentTimeMillis();
			final long montonicTimeAfter = Util.platformTimeMillis();
			System.out.println("...done");

			long expectedApplicationTimeAfter = applicationTimeBefore + timeOffset + delay;
			long expectedMonotonicTimeAfter = monotonicTimeBefore + delay;

			// Test that modifying the application time is correctly done
			final String assertionMessage = "Application time not correctly set. On some platforms, the MicroEJ Core Engine is not allowed to modify the time of the system (e.g., on Linux if the process does not have the right permissions). In such a case, you can ignore this test by setting the property '"
					+ PROPERTY_PREFIX + OPTION_CAN_SET_SYSTEM_TIME + "' to 'false'.";
			assertTrue(assertionMessage, applicationTimeAfter >= expectedApplicationTimeAfter
					&& applicationTimeAfter <= expectedApplicationTimeAfter + toleranceTime);

			// Test that modifying the application time does not impact the monotonic time.
			assertTrue("monotonic time not modified", montonicTimeAfter >= expectedMonotonicTimeAfter
					&& montonicTimeAfter <= expectedMonotonicTimeAfter + toleranceTime);

		} else {
			System.out.println("Setting application time not tested because the property "
					+ (PROPERTY_PREFIX + OPTION_CAN_SET_SYSTEM_TIME) + " is set to 'false'");
		}
	}

	/**
	 * Tests the <code>LLMJVM_IMPL_scheduleRequest</code> implementation.
	 */
	@Test
	public void testJavaRoundRobin() {
		System.out.println("-> Check Java round robin (LLMJVM_IMPL_scheduleRequest validation)...");
		System.out.println("For a best result, please disable all the C native tasks except the MicroEJ task.");
		int nbThreads = NB_THREADS;
		assertTrue("nbThreads >= 2", nbThreads >= 2);

		ROUND_ROBIN_TASK_COUNTERS = new long[nbThreads];
		int priority = Thread.currentThread().getPriority() - 1;
		Thread[] threads = new Thread[nbThreads];
		for (int i = threads.length; --i >= 0;) {
			Thread t = new Thread(new RoundRobinTestTask(i));
			threads[i] = t;
			t.setPriority(priority);
			t.start();
		}

		// Poll until all threads are waiting on the monitor
		while (RoundRobinTestTask.COUNTER < NB_THREADS) {
			try {
				Thread.sleep(500);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		System.out.println("Starting tasks and wait for " + (ROUND_ROBIN_TEST_DURATION / 1000) + " seconds...");
		synchronized (ROUND_ROBIN_LOCK) {
			ROUND_ROBIN_IS_RUNNING = true;
			ROUND_ROBIN_LOCK.notifyAll(); // start !
		}

		try {
			Thread.sleep(ROUND_ROBIN_TEST_DURATION);
		} catch (InterruptedException e1) {
			assertTrue("Unexpected InterruptedException", false);
		}

		// stop threads
		ROUND_ROBIN_IS_RUNNING = false;
		for (int i = threads.length; --i >= 0;) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				throw new Error();
			}
		}
		System.out.println("...done.");

		long totalCounter = 0;
		long minCounter = Long.MAX_VALUE;
		long maxCounter = 0;
		for (int i = threads.length; --i >= 0;) {
			long counter = ROUND_ROBIN_TASK_COUNTERS[i];
			totalCounter += counter;
			maxCounter = Math.max(maxCounter, counter);
			minCounter = Math.min(minCounter, counter);
		}

		long deltaCounter = maxCounter - minCounter;
		long averageCounter = totalCounter / threads.length;
		long deltaPercentage;
		if (minCounter > 0) {
			deltaPercentage = (deltaCounter * 100) / minCounter;
		} else {
			deltaPercentage = 100;
		}

		if (deltaPercentage > ROUND_ROBIN_MAX_DELTA_PERCENTAGE_ALLOWED) {
			// Print some information when the test fails
			System.out.println("Min counter = " + minCounter);
			System.out.println("Max counter = " + maxCounter);
			System.out.println("Average = " + averageCounter);
			System.out.println("Delta = " + deltaCounter);
			System.out.println("Delta Percentage = " + deltaPercentage);
		}

		assertTrue("counter increments < " + ROUND_ROBIN_MIN_COUNTER_REQUIRED + " (actually " + minCounter + ")",
				minCounter >= ROUND_ROBIN_MIN_COUNTER_REQUIRED);

		assertTrue("delta percentage > " + ROUND_ROBIN_MAX_DELTA_PERCENTAGE_ALLOWED + " (actually " + deltaPercentage
				+ ")", deltaPercentage <= ROUND_ROBIN_MAX_DELTA_PERCENTAGE_ALLOWED);
	}

	/**
	 * Tests the <code>LLMJVM_IMPL_scheduleRequest</code> implementation with a max schedule request time
	 * (Long.MAX_VALUE milliseconds).
	 *
	 * Tests Thread.sleep() with max number of milliseconds (Long.MAX_VALUE) does not cause an infinite loop in the
	 * MicroEJ Core.
	 *
	 * This test will check if the time conversion overflow is correctly handled in the LLMJVM_scheduleRequest()
	 * implementation. A correct implementation should saturate the time to the max value of microseconds or ticks in
	 * case of overflow.
	 */
	@Test
	public void testScheduleMaxTime() {
		Thread waitMaxTimeThread = new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					System.out.println("WaitMaxTimeThread starts sleeping for `Long.MAX_VALUE` milliseconds");
					Thread.sleep(Long.MAX_VALUE);
					assertTrue("Max sleep time reached!", false);
				} catch (InterruptedException e) {
					// interrupted
				}
			}
		});

		waitMaxTimeThread.start();

		System.out.println("Main thread starts sleeping for 1s..");
		try {
			Thread.sleep(1000);
			System.out.println("Main thread woke up!");
			waitMaxTimeThread.interrupt();
			waitMaxTimeThread.join();
			assertTrue("Main thread woke up and continued its execution: MicroEJ Core does not loop indefinitely",
					true);
		} catch (InterruptedException e) {
			throw new Error();
		}

	}

	/**
	 * Tests the <code>LLBSP_IMPL_isInReadOnlyMemory</code> implementation.
	 */
	@Test
	public void testIsInReadOnlyMemory() {
		System.out.println("-> Check isInReadOnlyMemory (LLBSP_IMPL_isInReadOnlyMemory validation)...");

		try {
			String s = "literal string";
			synchronized (s) {
				System.out.println("Test synchronize on literal string");
			}
		} catch (IllegalMonitorStateException e) {
			fail("IllegalMonitorStateException during synchronization on immutable object (literal string)");
		}

		try {
			synchronized (MicroejCoreValidation.class) {
				System.out.println("Test synchronize on class");
			}
		} catch (IllegalMonitorStateException e) {
			fail("IllegalMonitorStateException during synchronization on immutable object (class)");
		}

		/*
		 * There is a limit of the number of the synchronized monitors in flash but not in ram. We test this limit is
		 * reached for the objects in flash but not for the objects in ram. If the LLBSP_IMPL_isInReadOnlyMemory
		 * function is not correctly implemented this test can highlight it.
		 */
		System.out.println("Test multiple synchronize");
		tryToSynchronizeOn(objectsFromRam(), false);
		tryToSynchronizeOn(stringsFromImmutables(), true);
		tryToSynchronizeOn(classes(), true);
		tryToSynchronizeOn(objectsFromImmortals(), false);
		tryToSynchronizeOn(objectsFromImmutables(), true);
	}

	/**
	 * Tests the VEE Port FPU configuration.
	 */
	@Test
	public void testFPU() {
		System.out.println("-> Check FPU (soft/hard FP option)...");

		assertEquals("test 'float * float' in Java: " + INCOHERENT_FPU_MESSAGE, new Float(12f),
				new Float(testFPUJava(float3, float4)));
		assertEquals("test 'double * double' in Java: " + INCOHERENT_FPU_MESSAGE, new Double(12),
				new Double(testFPUJava(double3, double4)));
		assertEquals("test 'float * float' in C: " + INCOHERENT_FPU_MESSAGE, new Float(12f),
				new Float(testFloat(float3, float4)));
		assertEquals("test 'double * double' in C: " + INCOHERENT_FPU_MESSAGE, new Double(12),
				new Double(testDouble(double3, double4)));
	}

	/**
	 * Tests the VEE Port floating-point arithmetic.
	 */
	@Test
	public void testFloatingPointArithmetic() {
		System.out.println("-> Check floating-point arithmetic with NaN...");

		boolean allOk = true;

		allOk &= checkTrue("test 'NaN + float' returns NaN", Float.isNaN(floatNaN + float3));
		allOk &= checkTrue("test 'NaN - float' returns NaN", Float.isNaN(floatNaN - float3));
		allOk &= checkTrue("test 'NaN / float' returns NaN", Float.isNaN(floatNaN / float3));
		allOk &= checkTrue("test 'NaN * float' returns NaN", Float.isNaN(floatNaN * float3));
		allOk &= checkTrue("test 'NaN % float' returns NaN", Float.isNaN(floatNaN % float3));
		allOk &= checkTrue("test '(double)Float.NaN' returns NaN", Double.isNaN(floatNaN));
		allOk &= checkEquals("test '(int)Float.NaN' returns 0", 0, (int) floatNaN);
		allOk &= checkEquals("test '(long)Float.NaN' returns 0", 0, (long) floatNaN);

		allOk &= checkTrue("test 'NaN + double' returns NaN", Double.isNaN(doubleNaN + double3));
		allOk &= checkTrue("test 'NaN - double' returns NaN", Double.isNaN(doubleNaN - double3));
		allOk &= checkTrue("test 'NaN / double' returns NaN", Double.isNaN(doubleNaN / double3));
		allOk &= checkTrue("test 'NaN * double' returns NaN", Double.isNaN(doubleNaN * double3));
		allOk &= checkTrue("test 'NaN % double' returns NaN", Double.isNaN(doubleNaN % double3));
		allOk &= checkTrue("test '(float)Double.NaN' returns NaN", Float.isNaN((float) doubleNaN));
		allOk &= checkEquals("test '(int)Double.NaN' returns 0", 0, (int) doubleNaN);
		allOk &= checkEquals("test '(long)Double.NaN' returns 0", 0, (long) doubleNaN);

		System.out.println("-> Check floating-point arithmetic with 0.0 and -0.0...");
		allOk &= checkTrue("test 'float % 0.0' returns NaN", Float.isNaN(float3 % floatZero));
		allOk &= checkTrue("test 'float % -0.0' returns NaN", Float.isNaN(float3 % floatNegativeZero));
		allOk &= checkTrue("test 'double % 0.0' returns NaN", Double.isNaN(double3 % doubleZero));
		allOk &= checkTrue("test 'double % -0.0' returns NaN", Double.isNaN(double3 % doubleNegativeZero));

		allOk &= checkTrue("test 'inf * 0.0f' returns NaN", Float.isNaN(floatPositiveInfinity * floatZero));
		allOk &= checkEquals("test 'float * -0.0' returns -0.0", 0, Float.compare(-0.0f, (float3 * floatNegativeZero)));
		allOk &= checkEquals("test 'float * 0.0' returns 0.0", 0, Float.compare(0.0f, (float3 * floatZero)));
		allOk &= checkEquals("test '-float * -0.0' returns 0.0", 0, Float.compare(0.0f, (-float3 * floatNegativeZero)));
		allOk &= checkEquals("test '-float * 0.0' returns -0.0", 0, Float.compare(-0.0f, (-float3 * floatZero)));
		allOk &= checkTrue("test 'inf * 0.0d' returns NaN", Double.isNaN(doublePositiveInfinity * doubleZero));
		allOk &= checkEquals("test 'double * -0.0' returns -0.0", 0,
				Double.compare(-0.0, (double3 * doubleNegativeZero)));
		allOk &= checkEquals("test 'double * 0.0' returns 0.0", 0, Double.compare(0.0, (double3 * doubleZero)));
		allOk &= checkEquals("test '-double * -0.0' returns 0.0", 0,
				Double.compare(0.0, (-double3 * doubleNegativeZero)));
		allOk &= checkEquals("test '-double * 0.0' returns -0.0", 0, Double.compare(-0.0, (-double3 * doubleZero)));

		allOk &= checkEquals("test '-0.0f + 0.0f' returns 0.0f", 0, Float.compare(0.0f, floatNegativeZero + floatZero));
		allOk &= checkEquals("test '-0.0f - -0.0f' returns 0.0f", 0,
				Float.compare(0.0f, floatNegativeZero - floatNegativeZero));
		allOk &= checkEquals("test '-0.0d + 0.0d' returns 0.0d", 0,
				Double.compare(0.0d, doubleNegativeZero + doubleZero));
		allOk &= checkEquals("test '-0.0d - -0.0d' returns 0.0d", 0,
				Double.compare(0.0d, doubleNegativeZero - doubleNegativeZero));

		allOk &= checkEquals("test '(float)-0.0d' returns -0.0f", 0, Float.compare(-0.0f, (float) doubleNegativeZero));
		allOk &= checkEquals("test '(double)-0.0f' returns -0.0d", 0, Double.compare(-0.0, floatNegativeZero));

		System.out.println("-> Check floating-point arithmetic with infinity...");
		allOk &= checkTrue("test '+inf % float' returns NaN", Float.isNaN(floatPositiveInfinity % float3));
		allOk &= checkTrue("test '-inf % float' returns NaN", Float.isNaN(floatNegativeInfinity % float3));
		allOk &= checkTrue("test '-inf + float' returns -inf",
				(floatNegativeInfinity + float3) == Float.NEGATIVE_INFINITY);
		allOk &= checkTrue("test '+inf + -float' returns +inf",
				(floatPositiveInfinity + -float3) == Float.POSITIVE_INFINITY);
		allOk &= checkTrue("test '(double)+inf' returns +inf", floatPositiveInfinity == Double.POSITIVE_INFINITY);
		allOk &= checkTrue("test '(double)-inf' returns -inf", floatNegativeInfinity == Double.NEGATIVE_INFINITY);

		allOk &= checkTrue("test '+inf % double' returns NaN", Double.isNaN(doublePositiveInfinity % double3));
		allOk &= checkTrue("test '-inf % double' returns NaN", Double.isNaN(doubleNegativeInfinity % double3));
		allOk &= checkTrue("test '-inf + double' returns -inf",
				(doubleNegativeInfinity + double3) == Double.NEGATIVE_INFINITY);
		allOk &= checkTrue("test '+inf + -double' returns +inf",
				(doublePositiveInfinity + -double3) == Double.POSITIVE_INFINITY);
		allOk &= checkTrue("test '(float)+inf' returns +inf",
				(float) doublePositiveInfinity == Float.POSITIVE_INFINITY);
		allOk &= checkTrue("test '(float)-inf' returns -inf",
				(float) doubleNegativeInfinity == Float.NEGATIVE_INFINITY);

		System.out.println("-> Check floating-point arithmetic with min values...");
		allOk &= checkTrue("test 'min % float' returns min", (Float.MIN_VALUE % float3) == Float.MIN_VALUE);
		allOk &= checkTrue("test 'min % double' returns min", (Double.MIN_VALUE % float3) == Double.MIN_VALUE);

		System.out.println("-> Check floating-point division by 0.0...");
		allOk &= checkTrue("test '0.0f / 0.0f' returns NaN", Float.isNaN(floatZero / floatZero));
		allOk &= checkTrue("test 'float / 0.0' returns +inf", (float3 / floatZero) == Float.POSITIVE_INFINITY);
		allOk &= checkTrue("test 'float / -0.0' returns -inf", (float3 / floatNegativeZero) == Float.NEGATIVE_INFINITY);
		allOk &= checkTrue("test '-float / 0.0' returns -inf", (-float3 / floatZero) == Float.NEGATIVE_INFINITY);
		allOk &= checkTrue("test '-float / -0.0' returns +inf",
				(-float3 / floatNegativeZero) == Float.POSITIVE_INFINITY);

		allOk &= checkTrue("test '0.0d / 0.0d' returns NaN", Double.isNaN(doubleZero / doubleZero));
		allOk &= checkTrue("test 'double / 0.0' returns +inf", (double3 / doubleZero) == Double.POSITIVE_INFINITY);
		allOk &= checkTrue("test 'double / -0.0' returns -inf",
				(double3 / doubleNegativeZero) == Double.NEGATIVE_INFINITY);
		allOk &= checkTrue("test '-double / 0.0' returns -inf", (-double3 / doubleZero) == Double.NEGATIVE_INFINITY);
		allOk &= checkTrue("test '-double / -0.0' returns +inf",
				(-double3 / doubleNegativeZero) == Double.POSITIVE_INFINITY);

		System.out.println("-> Check floating-point Math functions...");
		allOk &= checkTrue("test 'log1p(-3)' returns NaN", Double.isNaN(Math.log1p(-double3)));
		allOk &= checkTrue("test 'sqrt(neg)' returns NaN", Double.isNaN(Math.sqrt(-double3)));
		allOk &= checkTrue("test 'IEEEremainder(double, 0.0)' returns NaN",
				Double.isNaN(Math.IEEEremainder(double3, doubleZero)));
		allOk &= checkTrue("test 'IEEEremainder(double, -0.0)' returns NaN",
				Double.isNaN(Math.IEEEremainder(double3, doubleNegativeZero)));
		allOk &= checkTrue("test 'cos(NaN)' returns NaN", Double.isNaN(Math.cos(doubleNaN)));
		allOk &= checkTrue("test 'cos(+inf)' returns NaN", Double.isNaN(Math.cos(doublePositiveInfinity)));
		allOk &= checkTrue("test 'cos(-inf)' returns NaN", Double.isNaN(Math.cos(doubleNegativeInfinity)));
		allOk &= checkTrue("test 'sin(NaN)' returns NaN", Double.isNaN(Math.sin(doubleNaN)));
		allOk &= checkTrue("test 'sin(+inf)' returns NaN", Double.isNaN(Math.sin(doublePositiveInfinity)));
		allOk &= checkTrue("test 'sin(-inf)' returns NaN", Double.isNaN(Math.sin(doubleNegativeInfinity)));
		allOk &= checkTrue("test 'tan(NaN)' returns NaN", Double.isNaN(Math.tan(doubleNaN)));
		allOk &= checkTrue("test 'tan(+inf)' returns NaN", Double.isNaN(Math.tan(doublePositiveInfinity)));
		allOk &= checkTrue("test 'tan(-inf)' returns NaN", Double.isNaN(Math.tan(doubleNegativeInfinity)));
		allOk &= checkTrue("test 'acos(NaN)' returns NaN", Double.isNaN(Math.acos(doubleNaN)));
		allOk &= checkTrue("test 'asin(NaN)' returns NaN", Double.isNaN(Math.asin(doubleNaN)));
		allOk &= checkTrue("test 'atan(NaN)' returns NaN", Double.isNaN(Math.atan(doubleNaN)));

		System.out.println("-> Check floating-point NaN bit pattern...");
		allOk &= checkEquals("test float NaN bit pattern", 0x7FC00000, Float.floatToRawIntBits(floatZero / floatZero));
		allOk &= checkEquals("test double NaN bit pattern", 0x7ff8000000000000L,
				Double.doubleToRawLongBits(doubleZero / doubleZero));

		System.out.println("-> Check integer arithmetic...");
		allOk &= checkEquals("test 'INT_MIN / -1' returns INT_MIN", Integer.MIN_VALUE, intMin / intNegativeOne);
		allOk &= checkEquals("test 'INT_MIN % -1' returns INT_MIN", 0, intMin % intNegativeOne);
		allOk &= checkEquals("test 'LONG_MIN / -1' returns LONG_MIN", Long.MIN_VALUE, longMin / longNegativeOne);
		allOk &= checkEquals("test 'LONG_MIN % -1' returns LONG_MIN", 0, longMin % longNegativeOne);

		assertTrue(allOk);
	}

	private static boolean checkTrue(String message, boolean condition) {
		try {
			assertTrue(message, condition);
			return true;
		} catch (AssertionError error) {
			System.out.println("Assertion failed: " + error.getMessage());
			return false;
		}
	}

	private static boolean checkEquals(String message, long expected, long actual) {
		try {
			assertEquals(message, expected, actual);
			return true;
		} catch (AssertionError error) {
			System.out.println("Assertion failed: " + error.getMessage());
			return false;
		}
	}

	/**
	 * Tests the VEE Port floating-point parser.
	 */
	@Test
	public void testParseFloatingPoint() {
		System.out.println("-> Check floating-point parser...");

		float parsedFloat = Float.parseFloat("1234.5");
		float expectedFloat = 1234.5f;
		assertEquals("test 'parse float string': strtof " + INVALID_C_FUNCTION_MESSAGE, expectedFloat, parsedFloat,
				getAssertFloatDelta(expectedFloat));

		double parsedDouble = Double.parseDouble("1234.5");
		double expectedDouble = 1234.5;
		assertEquals("test 'parse double string': strtod " + INVALID_C_FUNCTION_MESSAGE, expectedDouble, parsedDouble,
				getAssertDoubleDelta(expectedDouble));

		float parsedFloatScientific = Float.parseFloat("9.82E-22");
		float expectedFloatScientific = 9.82E-22f;
		assertEquals("test 'parse float string scientific notation': strtof " + INVALID_C_FUNCTION_MESSAGE,
				expectedFloatScientific, parsedFloatScientific, getAssertFloatDelta(expectedFloatScientific));

		double parsedDoubleScientific = Double.parseDouble("1.25E128");
		double expectedDoubleScientific = 1.25E128;
		assertEquals("test 'parse double string scientific notation': strtod " + INVALID_C_FUNCTION_MESSAGE,
				expectedDoubleScientific, parsedDoubleScientific, getAssertDoubleDelta(expectedDoubleScientific));
	}

	/**
	 * Tests the VEE Port floating-point formatter.
	 */
	@Test
	public void testFormatFloatingPoint() {
		System.out.println("-> Check floating-point formatter...");

		String floatToString = Float.toString(1234.5f);
		assertEquals("test 'float to string': snprintf " + INVALID_C_FUNCTION_MESSAGE, "1234.5", floatToString);

		String doubleToString = Double.toString(1234.5d);
		assertEquals("test 'double to string': snprintf " + INVALID_C_FUNCTION_MESSAGE, "1234.5", doubleToString);

		String floatScientificToString = Float.toString(1.11E-22f);
		assertTrue(
				"test 'float to string scientific notation (toString(1.11E-22f) is '" + floatScientificToString
						+ "')': snprintf " + INVALID_C_FUNCTION_MESSAGE,
				floatScientificToString.startsWith("1.1") && floatScientificToString.endsWith("E-22"));

		String doubleScientificToString = Double.toString(1.11E128d);
		assertTrue(
				"test 'double to string scientific notation (toString(1.11E128d) is '" + doubleScientificToString
						+ "')': snprintf " + INVALID_C_FUNCTION_MESSAGE,
				doubleScientificToString.startsWith("1.1") && doubleScientificToString.endsWith("E128"));
	}

	/**
	 * Tests parse double/float with potential dynamic allocation.
	 */
	@Test
	public void testParseDoubleStringHeap() {
		System.out.println(
				"-> Check parsing a string as a double ; in some systems such operations may allocate memory in the C heap (strtod, strtof, malloc implementation)...");

		double parsedDouble;
		double expectedDouble;
		float parsedFloat;
		float expectedFloat;
		parsedDouble = Double.parseDouble("1.7976931348623157E308");
		expectedDouble = 1.7976931348623157E308;
		assertEquals("test 'parse float/double string (1/10)': strtod " + INVALID_C_FUNCTION_MESSAGE, expectedDouble,
				parsedDouble, getAssertDoubleDelta(expectedDouble));

		parsedDouble = Double.parseDouble("4.9E-324");
		expectedDouble = 4.9E-324;
		assertEquals("test 'parse float/double string (2/10)': strtod " + INVALID_C_FUNCTION_MESSAGE, expectedDouble,
				parsedDouble, getAssertDoubleDelta(expectedDouble));

		parsedDouble = Double.parseDouble(
				"8456452315484210000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000009999999999999999999999999999999999999999999999999999999999999999040.005460000000000E-512");
		expectedDouble = 8.4E-323d;
		assertEquals("test 'parse float/double string (3/10)': strtod " + INVALID_C_FUNCTION_MESSAGE, expectedDouble,
				parsedDouble, getAssertDoubleDelta(expectedDouble));

		parsedFloat = Float.parseFloat("7.56042114E-33");
		expectedFloat = 7.56042114E-33f;
		assertEquals("test 'parse float/double string (4/10)': strtof " + INVALID_C_FUNCTION_MESSAGE, expectedFloat,
				parsedFloat, getAssertFloatDelta(expectedFloat));

		parsedFloat = Float.parseFloat("3.71806751E-19");
		expectedFloat = 3.71806751E-19f;
		assertEquals("test 'parse float/double string (5/10)': strtof " + INVALID_C_FUNCTION_MESSAGE, expectedFloat,
				parsedFloat, getAssertFloatDelta(expectedFloat));

		parsedFloat = Float.parseFloat("7.99279006E37");
		expectedFloat = 7.99279006E37f;
		assertEquals("test 'parse float/double string (6/10)': strtof " + INVALID_C_FUNCTION_MESSAGE, expectedFloat,
				parsedFloat, getAssertFloatDelta(expectedFloat));

		parsedFloat = Float.parseFloat("2.27187279E-38");
		expectedFloat = 2.27187279E-38f;
		assertEquals("test 'parse float/double string (7/10)': strtof " + INVALID_C_FUNCTION_MESSAGE, expectedFloat,
				parsedFloat, getAssertFloatDelta(expectedFloat));

		parsedDouble = Double.parseDouble("1.7976931348623157E308");
		expectedDouble = 1.7976931348623157E308;
		assertEquals("test 'parse float/double string (8/10)': strtod " + INVALID_C_FUNCTION_MESSAGE, expectedDouble,
				parsedDouble, getAssertDoubleDelta(expectedDouble));

		String strDouble = Double.toString(2.4375d);
		assertEquals("test 'double to string (9/10)': sprintf " + INVALID_C_FUNCTION_MESSAGE, "2.4375", strDouble);

		parsedDouble = Double.parseDouble("4.9E-324");
		expectedDouble = 4.9E-324;
		assertEquals("test 'parse float/double string (10/10)': strtod " + INVALID_C_FUNCTION_MESSAGE, expectedDouble,
				parsedDouble, getAssertDoubleDelta(expectedDouble));

	}

	/**
	 * From expected double value, returns the delta value to use when comparing two doubles.
	 */
	private static double getAssertDoubleDelta(double expectedResult) {
		return expectedResult * FLOAT_COMPARISON_TOLERANCE_PERCENT;
	}

	/**
	 * From expected float value, returns the delta value to use when comparing two floats.
	 */
	private static float getAssertFloatDelta(float expectedResult) {
		return expectedResult * FLOAT_COMPARISON_TOLERANCE_PERCENT;
	}

	/**
	 * Tests that the <code>LLMJVM_IMPL_getCurrentTime</code> implementation always increases.
	 */
	@Test
	public void testMonotonicTimeIncreases() {
		final long testDurationS = getOptionAsInt(OPTION_MONOTONIC_CHECK_NB_SECONDS, 60, "second");
		System.out.println("-> Check monotonic time consistency for " + testDurationS
				+ " seconds (LLMJVM_IMPL_getCurrentTime)...");

		final long printDotPeriodMs = 2000;
		long startTime = Util.platformTimeMillis();
		long endTime = startTime + (testDurationS * 1000);
		long printDotTime = startTime + printDotPeriodMs;
		long previousTime = startTime;
		long currentTime;
		while (endTime > (currentTime = Util.platformTimeMillis())) {
			if (printDotTime < currentTime) {
				System.out.print('.');
				printDotTime = currentTime + printDotPeriodMs;
			}
			if (currentTime < previousTime) {
				System.out.println();
				assertTrue("Monotonic time goes back in time (currentTime = " + currentTime + " previousTime="
						+ previousTime + ").\nThis issue is usually caused by a non-atomic calculation of the time.",
						false);
				return;
			}
			previousTime = currentTime;
		}
		System.out.println();

	}

	/**
	 * Checks <code>LLMJVM_IMPL_getCurrentTime()</code> and <code>LLMJVM_IMPL_getTimeNanos()</code> clock tick duration.
	 */
	@Test
	public void testSystemCurrentTimeClockTick() {
		System.out.println(
				"-> Check current time clock tick duration (LLMJVM_IMPL_getCurrentTime, LLMJVM_IMPL_getTimeNanos)...");
		final long precisionLimitMs = getOptionAsInt(OPTION_MAX_ALLOWED_CLOCK_TICK_DURATION_MS,
				DEFAULT_MAX_ALLOWED_CLOCK_TICK_DURATION_MS, "milliseconds");

		long t0;
		long t1;
		long precision;

		// Check LLMJVM_IMPL_getCurrentTime
		t0 = System.currentTimeMillis();
		while ((t1 = System.currentTimeMillis()) == t0) {
			// Nothing to do
		}
		precision = t1 - t0;
		System.out.println("Estimated LLMJVM_IMPL_getCurrentTime clock tick is " + precision + " ms.");
		assertTrue("LLMJVM_IMPL_getCurrentTime timer precision (" + precision
				+ " ms) is lower than the expected limit (" + precisionLimitMs + " ms)", precision <= precisionLimitMs);

		// Check LLMJVM_IMPL_getTimeNanos
		t0 = System.nanoTime();
		t1 = System.nanoTime();
		if (t0 != t1) {
			// Time to call nanoTime is longer than the clock tick.
			// Cannot compute exact precision, just print this result
			System.out.println("Estimated LLMJVM_IMPL_getTimeNanos clock tick is lower than " + (t1 - t0) + " ns.");
		} else {
			t0 = System.nanoTime();
			while ((t1 = System.nanoTime()) == t0) {
				// Nothing to do
			}
			precision = t1 - t0;
			long precisionLimitNs = precisionLimitMs * 1000000l;
			System.out.println("Estimated LLMJVM_IMPL_getTimeNanos clock tick is " + precision + " ns.");
			assertTrue("LLMJVM_IMPL_getTimeNanos timer precision (" + precision
					+ " ns) is lower than the expected limit (" + precisionLimitNs + " ns)",
					precision <= precisionLimitNs);
		}

	}

	/**
	 * Checks <code>LLMJVM_IMPL_scheduleRequest()</code> clock tick duration.
	 */
	@Test
	public void testScheduleRequestClockTick() {
		System.out.println("-> Check schedule request clock tick duration (LLMJVM_IMPL_scheduleRequest)...");
		final long precisionLimit = getOptionAsInt(OPTION_MAX_ALLOWED_CLOCK_TICK_DURATION_MS,
				DEFAULT_MAX_ALLOWED_CLOCK_TICK_DURATION_MS, "milliseconds");
		try {
			// Execute a first sleep just to end the current clock cycle.
			// Following operations will start at the beginning of the next clock cycle.
			Thread.sleep(1);
			long t0 = System.currentTimeMillis();
			Thread.sleep(1);
			long t1 = System.currentTimeMillis();

			long precision = t1 - t0;
			System.out.println("Estimated LLMJVM_IMPL_scheduleRequest clock tick is " + precision + " ms.");
			assertTrue(
					"LLMJVM_IMPL_scheduleRequest timer precision (" + precision
							+ " ms) is lower than the expected limit (" + precisionLimit + " ms)",
					precision <= precisionLimit);

		} catch (InterruptedException e) {
			throw new Error();
		}
	}

	/**
	 * Checks SNI calling convention ABI.
	 */
	@Test
	public void testSniAbi() {
		System.out.println("-> Check SNI native calling convention (ABI)...");

		boolean allOk = true;

		int res01 = testNativeArguments01(0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10, 0x11121314, 0x15161718,
				0x191A1B1C, 0x1D1E1F20, 0x21222324, 0x25262728);
		allOk &= checkTrue("test int32_t SNI arguments", res01 == 0x292A2B2C);

		long res02 = testNativeArguments02(0x2D2E2F3031323334l, 0x35363738393A3B3Cl, 0x3D3E3F4041424344l,
				0x45464748494A4B4Cl, 0x4D4E4F5051525354l, 0x55565758595A5B5Cl, 0x5D5E5F6061626364l, 0x65666768696A6B6Cl,
				0x6D6E6F7071727374l, 0x75767778797A7B7Cl);
		allOk &= checkTrue("test int64_t SNI arguments", res02 == 0x7D7E7F8081828384l);

		long res03 = testNativeArguments03(0x85868788, 0x898A8B8C8D8E8F90l, 0x91929394, 0x95969798999A9B9Cl, 0x9D9E9FA0,
				0xA1A2A3A4A5A6A7A8l, 0xA9AAABAC, 0xADAEAFB0B1B2B3B4l, 0xB5B6B7B8, 0xB9BABBBCBDBEBFC0l);
		allOk &= checkTrue("test int32_t/int64_t SNI arguments", res03 == 0xC1C2C3C4C5C6C7C8l);

		float res04 = testNativeArguments04(1.0f, 1.1f, 1.2f, 1.3f, 1.4f, 1.5f, 1.6f, 1.7f, 1.8f, 1.9f);
		allOk &= checkTrue("test float SNI arguments", res04 == 2.0f);

		double res05 = testNativeArguments05(2.0d, 2.1d, 2.2d, 2.3d, 2.4d, 2.5d, 2.6d, 2.7d, 2.8d, 2.9d);
		allOk &= checkTrue("test double SNI arguments", res05 == 3.0d);

		double res06 = testNativeArguments06(3.0f, 3.1d, 3.2f, 3.3d, 3.4f, 3.5d, 3.6f, 3.7d, 3.8f, 3.9d);
		allOk &= checkTrue("test float/double SNI arguments", res06 == 4.0d);

		assertTrue(allOk);
	}

	/**
	 * Checks SNI atomic exchange.
	 */
	@Test
	public void testSniAtomicExchange() {
		System.out.println("-> Check SNI atomic exchange implementation (LLBSP_IMPL_atomic_exchange)...");

		// Suspend a Thread with SNI and resume it from another thread.
		// If the thread is not suspended or the sleep duration is not correct,
		// then there is an error in the implementation of LLBSP_IMPL_atomic_exchange().

		final long RESUME_DELAY = 2000;
		final long SUSPEND_TIMEOUT = 5000;
		final long DELAY_TOLERANCE = 500;

		long t0 = ej.bon.Util.platformTimeMillis();

		int threadId = sniGetCurrentThreadID();
		asyncResumeThread(threadId, RESUME_DELAY);

		sniSuspend(SUSPEND_TIMEOUT);

		long t1 = ej.bon.Util.platformTimeMillis();

		long suspendDuration = t1 - t0;

		// Check that the thread has been suspended
		assertTrue(
				"SNI suspend duration (" + suspendDuration
						+ ") is lower than expected: issue in LLBSP_IMPL_atomic_exchange",
				suspendDuration > (RESUME_DELAY - DELAY_TOLERANCE));
		// Check that the thread has been resumed by the other thread and not by the timeout
		assertTrue(
				"SNI suspend duration (" + suspendDuration
						+ ") is higher than expected: issue in LLBSP_IMPL_atomic_exchange",
				suspendDuration < (RESUME_DELAY + DELAY_TOLERANCE));
	}

	/**
	 * Resumes the given thread after the given delay.
	 * <p>
	 * Starts a thread that calls SNI_resumeJavaThread() after a the delay.
	 */
	private static void asyncResumeThread(final int threadId, final long delayMs) {
		Runnable runnable = new Runnable() {
			@Override
			public void run() {
				try {
					Thread.sleep(delayMs);
				} catch (InterruptedException e) {
					// Unexpected error
					e.printStackTrace();
					return;
				}
				sniResume(threadId);
			}
		};

		new Thread(runnable).start();

	}

	/**
	 * Task class for the round robin test.
	 */
	private static class RoundRobinTestTask implements Runnable {

		public static int COUNTER = 0;

		private final int id;

		public RoundRobinTestTask(int id) {
			this.id = id;
		}

		@Override
		public void run() {
			synchronized (MicroejCoreValidation.ROUND_ROBIN_LOCK) {
				if (!MicroejCoreValidation.ROUND_ROBIN_IS_RUNNING) {
					++COUNTER;
					System.out.println("Task " + this.id + " is waiting for start...");
					try {
						MicroejCoreValidation.ROUND_ROBIN_LOCK.wait();
					} catch (InterruptedException e) {
						throw new Error();
					}
				}
			}
			while (MicroejCoreValidation.ROUND_ROBIN_IS_RUNNING) {
				++MicroejCoreValidation.ROUND_ROBIN_TASK_COUNTERS[this.id];
			}
			System.out.println("Task " + this.id + " ends.");
		}

	}

	/**
	 * Synchronizes on a list of monitors and sleeps for a while.
	 */
	private static class MonitorKeeper implements Runnable {
		static final int THREAD_COUNT = 5;
		static final int MONITOR_PER_THREAD_COUNT = 7;
		static final int SLEEP_TIME = 5_000;
		public static int errorCount;

		private final Object[] monitors;

		public MonitorKeeper(Object[] monitors) {
			this.monitors = monitors;
		}

		@Override
		public void run() {
			try {
				synchronizeAll(0);
			} catch (Error e) {
				errorCount++;
			}
		}

		private void synchronizeAll(int monitorIndex) {
			if (monitorIndex < this.monitors.length) {
				synchronized (this.monitors[monitorIndex]) {
					synchronizeAll(monitorIndex + 1);
				}
			} else {
				try {
					Thread.sleep(SLEEP_TIME);
				} catch (InterruptedException e) {
					// Nothing to do here.
				}
			}
		}

	}

	// See instructions in README.rst for the implementation of the following native methods.

	private static native float testFloat(float a, float b);

	private static native double testDouble(double a, double b);

	private static native int testNativeArguments01(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8,
			int i9, int i10);

	private static native long testNativeArguments02(long l1, long l2, long l3, long l4, long l5, long l6, long l7,
			long l8, long l9, long l10);

	private static native long testNativeArguments03(int i1, long l2, int i3, long l4, int i5, long l6, int i7, long l8,
			int i9, long l10);

	private static native float testNativeArguments04(float f1, float f2, float f3, float f4, float f5, float f6,
			float f7, float f8, float f9, float f10);

	private static native double testNativeArguments05(double d1, double d2, double d3, double d4, double d5, double d6,
			double d7, double d8, double d9, double d10);

	private static native double testNativeArguments06(float f1, double d2, float f3, double d4, float f5, double d6,
			float f7, double d8, float f9, double d10);

	private static native int sniSuspend(long timeout);

	private static native int sniResume(int threadID);

	private static native int sniGetCurrentThreadID();

}
