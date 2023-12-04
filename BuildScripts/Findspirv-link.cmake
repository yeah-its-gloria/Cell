# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

include(FindPackageHandleStandardArgs)

find_program(spirv-link_PROGRAM NAMES spirv-link HINTS $ENV{VULKAN_SDK}/Bin)
find_package_handle_standard_args(spirv-link DEFAULT_MSG spirv-link_PROGRAM)
