# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

include(FindPackageHandleStandardArgs)

find_path   (wayland_INCLUDE_DIR NAMES wayland-client.h)
find_library(wayland_LIBRARY     NAMES wayland-client)

find_path   (xkbcommon_INCLUDE_DIR NAMES xkbcommon/xkbcommon.h)
find_library(xkbcommon_LIBRARY     NAMES xkbcommon)

find_package_handle_standard_args(wayland DEFAULT_MSG wayland_LIBRARY xkbcommon_LIBRARY wayland_INCLUDE_DIR xkbcommon_INCLUDE_DIR)

if (wayland_FOUND)
    set(wayland_LIBRARIES "${wayland_LIBRARY};${xkbcommon_LIBRARY}")
    set(wayland_INCLUDE_DIRS "${wayland_INCLUDE_DIR}:${xkbcommon_INCLUDE_DIR}")
endif ()

mark_as_advanced(wayland_INCLUDE_DIR xkbcommon_INCLUDE_DIR wayland_LIBRARY xkbcommon_LIBRARY)
