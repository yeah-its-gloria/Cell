# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

include(FindPackageHandleStandardArgs)

find_path   (xkbcommon_INCLUDE_DIR NAMES xkbcommon/xkbcommon.h)
find_library(xkbcommon_LIBRARY     NAMES xkbcommon)

find_package_handle_standard_args(xkbcommon DEFAULT_MSG xkbcommon_LIBRARY xkbcommon_INCLUDE_DIR)

if(xkbcommon_FOUND)
	set(xkbcommon_LIBRARIES    "${xkbcommon_LIBRARY}")
	set(xkbcommon_INCLUDE_DIRS "${xkbcommon_INCLUDE_DIR}")
endif()

mark_as_advanced(xkbcommon_INCLUDE_DIR xkbcommon_LIBRARY)
