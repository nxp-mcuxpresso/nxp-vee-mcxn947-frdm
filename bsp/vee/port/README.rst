.. 
    Copyright 2022-2024 MicroEJ Corp. All rights reserved.
    Use of this source code is governed by a BSD-style license that can be found with this software.

.. |BOARD_NAME| replace:: FRDM-MCXN947
.. |PLATFORM| replace:: NXP VEE FRDM-MCXN947
.. |VEEPORT| replace:: VEE Port
.. |RTOS| replace:: FreeRTOS
.. |BSP_FULL_NAME| replace:: MCUXpresso Software Development Kit
.. |BSP_SHORT_NAME| replace:: SDK_2_12_1_RDRW610

.. _README: ./../../../README.md

================
|BOARD_NAME| BSP
================

This project contains the BSP sources of the |PLATFORM| for the
|BOARD_NAME|.

This document does not describe how to setup the |PLATFORM|. Please
refer to the `README`_ for that.

Build & Run Scripts
---------------------

In the directory ``scripts/`` are scripts that can be
used to build and flash the BSP.  The ``.bat`` and ``.sh`` scripts are
meant to run in a Windows and Linux environment respectively.

- The ``build*`` scripts are used to compile and link the BSP with a
  MicroEJ Application to produce a MicroEJ Firmware
  (``application.{out,hex,bin}``) that can be flashed on a device.

  The ``build*`` scripts work out of the box, assuming the toolchain, make, cmake and ninja are installed.

- The ``run*`` scripts are used to flash a MicroEJ Firmware
  (``application.{out,hex,bin}``) on a device.

The following environment variables are customizable:  

- ``RELEASE``: Build mode for the BSP. ``1`` for RELEASE mode and ``0`` for DEBUG mode.
  Default value: ``RELEASE``.
- ``FLASH_CMD``: |BOARD_NAME| board can eather be flashed through the USB port using LinkServer or throung the JTAG connector using J-Link.
  Set ``flash_cmsisdap`` for LinkServer probe or ``flash`` for J-Link probe.
  Default value: ``flash_cmsisdap``.
- ``ARMGCC_DIR``: Installation directory for the GCC toolchain.

The environment variables can be defined by the user in
the ``set_project_env*`` scripts.  When the ``.bat`` (``.sh``) scripts
are executed, the ``set_project_env.bat`` (``set_project_env.sh``) script
is executed if it exists.  Create and configure these files to
customize the environment locally.

Customize BSP
-------------

Configuration files for this BSP are located in ``nxp-vee-mcxn947-frdm\bsp\projects\nxpvee-ui\bsp`` folder.
Compilation flags are defined in ``nxp-vee-mcxn947-frdm\bsp\projects\nxpvee-ui\armgcc\flags.cmake``.
CMake modules used by this BSP are declared in ``nxp-vee-mcxn947-frdm\bsp\projects\nxpvee-ui\armgcc\CMakeLists.txt``.
Linker script is located here ``nxp-vee-mcxn947-frdm\bsp\projects\nxpvee-ui\armgcc\MCXN947_cm33_core0_flash.ld``.

Flash the Board
---------------

The ``run*`` scripts can also be used to flash the device with the MicroEJ Firmware.
