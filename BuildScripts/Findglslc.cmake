# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

include(FindPackageHandleStandardArgs)

find_program(glslc_PROGRAM NAMES glslc HINTS $ENV{VULKAN_SDK}/Bin)
find_package_handle_standard_args(glslc DEFAULT_MSG glslc_PROGRAM)
