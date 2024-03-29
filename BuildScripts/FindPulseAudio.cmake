# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

include(FindPackageHandleStandardArgs)

find_path   (PulseAudio_INCLUDE_DIR   NAMES pulse/pulseaudio.h)
find_library(PulseAudio_LIBRARY       NAMES pulse)

find_package_handle_standard_args(PulseAudio DEFAULT_MSG PulseAudio_LIBRARY PulseAudio_INCLUDE_DIR)

if (PulseAudio_FOUND)
    set(PulseAudio_LIBRARIES "${PulseAudio_LIBRARY}")
    set(PulseAudio_INCLUDE_DIRS "${PulseAudio_INCLUDE_DIR}")
endif ()

mark_as_advanced(PulseAudio_INCLUDE_DIR PulseAudio_LIBRARY)
