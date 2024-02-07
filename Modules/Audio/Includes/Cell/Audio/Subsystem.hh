// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/Audio/Result.hh>
#include <Cell/System/String.hh>

namespace Cell::Audio {

// Types of supported formats.
enum class FormatType : uint8_t {
    // 32 bit PCM stored in floating point.
    Float32PCM,

    // 16 bit PCM stored as signed integers.
    S16PCM
};

// Describes a bare audio format. format.
struct Format {
    FormatType type;
    uint8_t channels;

    // In Hertz.
    uint32_t rate;
};

class IRenderer;

// Base audio subsystem interface.
class ISubsystem : public Object {
public:
    // Common destructor.
    virtual ~ISubsystem() = default;

    // Sets up resources for rendering audio.
    virtual Wrapped<IRenderer*, Result> CreateRenderer(Format format) = 0;
};

// Sets up the most suitable subsystem for the current platform.
CELL_FUNCTION Wrapped<ISubsystem*, Result> CreateSubsystem(const System::String& title);

}