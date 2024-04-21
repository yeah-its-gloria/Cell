# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

include(FindPackageHandleStandardArgs)

if (WIN32)
    find_file(Vulkan_BINARY NAMES vulkan-1.dll)
elseif (APPLE)
    # TODO: find a less hard coded way to find the relevant dylib (needed for proper packaging later)
    find_file(Vulkan_BINARY NAMES libvulkan.1.3.280.dylib HINTS /usr/local/lib NO_CACHE)
endif ()

find_path   (Vulkan_INCLUDE_DIR NAMES vulkan/vulkan.h HINTS $ENV{VULKAN_SDK}/Include)
find_library(Vulkan_LIBRARY     NAMES vulkan-1 vulkan HINTS $ENV{VULKAN_SDK}/Lib)

find_package_handle_standard_args(Vulkan DEFAULT_MSG Vulkan_LIBRARY Vulkan_INCLUDE_DIR)

if (Vulkan_FOUND)
    if (NOT ${Vulkan_BINARY} STREQUAL "Vulkan_BINARY-NOTFOUND")
        set(Vulkan_BINARIES "${Vulkan_BINARY}")
    endif ()

    set(Vulkan_LIBRARIES "${Vulkan_LIBRARY}")
    set(Vulkan_INCLUDE_DIRS "${Vulkan_INCLUDE_DIR}")
endif ()

mark_as_advanced(Vulkan_BINARY Vulkan_INCLUDE_DIR Vulkan_LIBRARY)
