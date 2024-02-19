# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

include(FindPackageHandleStandardArgs)

find_package(WindowsSDK REQUIRED FATAL_ERROR)

find_program(dxc_PROGRAM NAMES dxc HINTS "${WINDOWS_SDK_ROOT}\\bin\\${WINDOWS_SDK_VERSION}\\x64")
find_package_handle_standard_args(dxc DEFAULT_MSG dxc_PROGRAM)
