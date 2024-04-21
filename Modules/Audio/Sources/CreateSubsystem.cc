// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/Subsystem.hh>
#include <Cell/System/Panic.hh>

#ifdef CELL_PLATFORM_WINDOWS
#include <Cell/Audio/Implementations/WASAPI.hh>
#elif CELL_PLATFORM_MACOS
#include <Cell/Audio/Implementations/CoreAudio.hh>
#elif CELL_PLATFORM_LINUX
#include <Cell/Audio/Implementations/PulseAudio.hh>
#else
#error No implementation!
#endif

namespace Cell::Audio {

Wrapped<ISubsystem*, Result> CreateSubsystem(const String& title) {
#ifdef CELL_PLATFORM_WINDOWS
    Wrapped<Implementations::WASAPI::Subsystem*, Result> result = Implementations::WASAPI::Subsystem::New(title);
#elif CELL_PLATFORM_MACOS
    Wrapped<Implementations::CoreAudio::Subsystem*, Result> result = Implementations::CoreAudio::Subsystem::New(title);
#elif CELL_PLATFORM_LINUX
    Wrapped<Implementations::PulseAudio::Subsystem*, Result> result = Implementations::PulseAudio::Subsystem::New(title);
#else
#define unimpl
    (void)(title);

    CELL_UNIMPLEMENTED
#endif

#ifndef unimpl
    if (!result.IsValid()) {
        return result.Result();
    }

    return result.Unwrap();
#else
#undef unimpl
#endif
}

}
