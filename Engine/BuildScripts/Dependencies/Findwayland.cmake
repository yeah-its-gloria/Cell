# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

include(FindPackageHandleStandardArgs)

find_path   (wayland_INCLUDE_DIR NAMES wayland-client.h)
find_library(wayland_LIBRARY     NAMES wayland-client)

find_package_handle_standard_args(wayland DEFAULT_MSG wayland_LIBRARY wayland_INCLUDE_DIR)

if(wayland_FOUND)
	set(wayland_LIBRARIES    "${wayland_LIBRARY}")
	set(wayland_INCLUDE_DIRS "${wayland_INCLUDE_DIR}")
endif()

mark_as_advanced(wayland_BINARY wayland_INCLUDE_DIR wayland_LIBRARY)
