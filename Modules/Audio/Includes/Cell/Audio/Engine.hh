// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/Audio/Result.hh>

namespace Cell::Audio {

enum class FormatType : uint8_t {
    // 32 bit PCM stored in floating point.
    Float32PCM
};

struct Format {
    FormatType type;

    // Supports Stereo only.
    uint8_t channels;

    // In Hertz.
    uint32_t rate;
};

// Base audio engine interface.
class IEngine : public Object {
public:
    // Common destructor.
    virtual ~IEngine() = default;

    // Sets up resources for rendering audio.
    virtual Result SetUpRendering(Format format) = 0;

    // Writes the given number of samples to the subsystem in one go.
    virtual Result WriteSamples(const uint8_t* CELL_NONNULL data, const uint32_t count, const bool treatAsSilent = false) = 0;

    // Begins playback.
    virtual Result PlaybackBegin() = 0;

    // Ends playback.
    virtual Result PlaybackEnd() = 0;

    // Returns the size of the allocated buffer.
    virtual Wrapped<uint32_t, Result> GetBufferSize() = 0;

    // Returns the number of samples left in the buffer.
    virtual Wrapped<uint32_t, Result> GetCurrentBufferFillCount() = 0;

    // Returns the latency of the subsystem in microseconds. Useful for waiting between submissions.
    virtual uint32_t GetLatencyMicroseconds() = 0;
};

// Sets up the most suited audio engine for the current platform.
CELL_FUNCTION Wrapped<IEngine*, Result> CreateEngine();

}
