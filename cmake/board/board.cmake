#
# Copyright 2023 NXP
#
# SPDX-License-Identifier: Apache-2.0
#

board_runner_args(jlink "--device=MCXN947_M33_0" "--reset-after-load")

include(cmake/board/jlink.board.cmake)
