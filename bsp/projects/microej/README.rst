.. 
    Copyright 2022-2023 MicroEJ Corp. All rights reserved.
    Use of this source code is governed by a BSD-style license that can be found with this software.

.. |BOARD_NAME| replace:: RW612-BGA EVK
.. |VEEPORT| replace:: VEE Port
.. |RTOS| replace:: FreeRTOS
.. |BSP_FULL_NAME| replace:: MCUXpresso Software Development Kit
.. |BSP_SHORT_NAME| replace:: SDK_2_12_1_RDRW610

.. _README: ./../../../README.rst

================
|BOARD_NAME| BSP
================

This project contains the BSP sources of the |VEEPORT| for the
|BOARD_NAME|. The BSP project is based on |BSP_FULL_NAME| (|BSP_SHORT_NAME|).

This document does not describe how to setup the |VEEPORT|. Please
refer to the `README`_ for that.

Build & Run Scripts
---------------------

In the directory ``Project/microej/scripts/`` are scripts that can be
used to build and flash the BSP.  The ``.bat`` and ``.sh`` scripts are
meant to run in a Windows and Linux environment respectively.

- The ``build*`` scripts are used to compile and link the BSP with a
  MicroEJ Application to produce a MicroEJ Firmware
  (``application.out``) that can be flashed on a device.

  The ``build*`` scripts work out of the box, assuming the toolchain is
  installed in the default path.

- The ``run*`` scripts is not available yet.

The following environment variables are customizable:  

**IAR toolchain**

- ``IAREW_INSTALLATION_DIR``: The path to IAR installation directory (already set to the default IAR Workbench default installation directory).
- ``IAREW_PROJECT_CONFIGURATION``: The project configuration (``Debug`` or ``Release``).
- ``IAREW_PROJECT_DIR``: The directory that contains the ``application.eww`` IAR project file (set to ``%~dp0``: the directory that contains the executed ``.bat``).
- ``IAREW_PROJECT_NAME``: The Eclipse CDT project name (``application`` by default).
- ``IAREW_OUT_FILENAME``: The output file name.
- ``IAREW_PROJECT_DEVICE_MACRO``: Device macro file depending on the board.
- ``IAREW_PROJECT_FLASH_LOADER``: Device flash loader file depending on the board.

The environment variables can be defined globally by the user or in
the ``set_local_env*`` scripts.  When the ``.bat`` (``.sh``) scripts
are executed, the ``set_local_env.bat`` (``set_local_env.sh``) script
is executed if it exists.  Create and configure these files to
customize the environment locally.  Template files are provided as
example, see ``set_local_env.bat.tpl`` and ``set_local_env.sh.tpl``.

Customize BSP
-------------

Flash the Board
---------------

To flash the board with IAR, go to ``Project > Download > Download active application``.

Debugging with |BOARD_NAME|
---------------------------

- Open the file ``-bsp/projects/microej/EWARM/set_project_env.bat``.
- Set the target configuration to ``Debug``.

..  code-block::

      SET IAREW_PROJECT_CONFIGURATION=Debug

- Open the IAR project in IAR Workbench (open the file ``-bsp/projects/microej/iar/application.eww`` from IAR Workbench or by double-clicking on it from the MicroEJ SDK).
- Ensure that the ``debug`` target is selected in the workspace tab
- Build and link the firmware: Right-click on the ``application`` project > ``Make`` or Press 'F7'
- Connect the |BOARD_NAME| to your computer
- Start the debug session by clicking on ``Project`` > ``Download and Debug``

Running EmbUnit & CoreMark benchs
=================================

To execute the EmbUnit and CoreMark benchs:

- Open the IAR project in IAR Workbench (open the file ``-bsp/projects/microej/iar/application.eww`` from IAR Workbench or by double-clicking on it from the MicroEJ SDK).
- Select the ``UnitTest`` target is selected in the workspace tab.
- Build and link the firmware: Right-click on the ``application`` project > ``Make`` or Press 'F7'.
- Connect the |BOARD_NAME| to your computer.
- Start the debug session by clicking on ``Project`` > ``Download and Debug``.

Troubleshooting
===============

Can not connect to the board with J-LINK
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- To unlock the board, remove jumper 3-4 of HD12, you can be able to connect J-Link and flash a correct image and put the jumper back to boot up. 
