// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#ifdef CELL_PLATFORM_WINDOWS

#include <Cell/Audio/WASAPI.hh>

#elif CELL_PLATFORM_LINUX
#include <Cell/Audio/PulseAudio.hh>
#endif

namespace Cell::Audio {

Wrapped<IEngine*, Result> CreateEngine() {
#ifdef CELL_PLATFORM_WINDOWS
    Wrapped<WASAPI*, Result> result = WASAPI::New();
#elif CELL_PLATFORM_LINUX
    Wrapped<PulseAudio*, Result> result = PulseAudio::New();
#else
    CELL_UNIMPLEMENTED
#endif

    if (!result.IsValid()) {
        return result.Result();
    }

    return result.Unwrap();
}

}
