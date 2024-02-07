// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#ifdef CELL_PLATFORM_WINDOWS
#include <Cell/Audio/Implementations/WASAPI.hh>
#elif CELL_PLATFORM_LINUX
#include <Cell/Audio/Implementations/PulseAudio.hh>
#else
#error No backing implementation!
#endif

namespace Cell::Audio {
using namespace Implementations;

Wrapped<ISubsystem*, Result> CreateSubsystem(const System::String& title) {
#ifdef CELL_PLATFORM_WINDOWS
    Wrapped<WASAPI::Subsystem*, Result> result = WASAPI::Subsystem::New(title);
#elif CELL_PLATFORM_LINUX
    Wrapped<PulseAudio::Subsystem*, Result> result = PulseAudio::Subsystem::New(title);
#else
    CELL_UNIMPLEMENTED
#endif

    if (!result.IsValid()) {
        return result.Result();
    }

    return result.Unwrap();
}

}
