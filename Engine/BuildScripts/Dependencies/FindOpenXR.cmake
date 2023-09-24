# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

include(FindPackageHandleStandardArgs)

if (WIN32)
    find_file(OpenXR_BINARY NAMES openxr_loader.dll)
endif ()

find_path   (OpenXR_INCLUDE_DIR NAMES openxr/openxr.h)
find_library(OpenXR_LIBRARY     NAMES openxr_loader)

find_package_handle_standard_args(OpenXR DEFAULT_MSG OpenXR_LIBRARY OpenXR_INCLUDE_DIR)

if (OpenXR_FOUND)
    if (NOT ${OpenXR_BINARY} STREQUAL "OpenXR_BINARY-NOTFOUND")
        set(OpenXR_BINARIES "${OpenXR_BINARY}")
    endif ()

    set(OpenXR_LIBRARIES "${OpenXR_LIBRARY}")
    set(OpenXR_INCLUDE_DIRS "${OpenXR_INCLUDE_DIR}")
endif ()

mark_as_advanced(OpenXR_BINARY OpenXR_INCLUDE_DIR OpenXR_LIBRARY)
