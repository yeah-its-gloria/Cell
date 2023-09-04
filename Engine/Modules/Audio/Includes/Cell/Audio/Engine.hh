// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/Audio/Result.hh>

namespace Cell::Audio {

// Base audio engine interface.
class IEngine : public Object {
public:
    // Common destructor.
    virtual ~IEngine() = default;

    // Sets up resources for rendering audio.
    virtual Result SetUpRendering() = 0;

    // Returns the maximum number of samples allowed in a single sample submission.
    virtual Wrapped<uint32_t, Result> GetMaximumSampleCount() = 0;

    // Returns the current sample offset for the subsystem.
    virtual Wrapped<uint32_t, Result> GetCurrentSampleOffset() = 0;

    // Writes the given number of samples to the subsystem in one go.
    virtual Result WriteSamples(const uint8_t* CELL_NONNULL data, const uint32_t count, const bool treatAsSilent = false) = 0;

    // Begins playback.
    virtual Result PlaybackBegin() = 0;

    // Ends playback.
    virtual Result PlaybackEnd() = 0;

    // Returns the sample count in bytes.
    virtual uint16_t GetSampleSizeInBytes() = 0;

    // Returns the number of samples per second.
    virtual uint16_t GetSampleCountPerSecond() = 0;

    // Returns the latency of the subsystem in microseconds. Useful for waiting between submissions.
    virtual uint32_t GetLatencyMicroseconds() = 0;
};

// Sets up the most suited audio engine for the current platform.
CELL_FUNCTION Wrapped<IEngine*, Result> CreateEngine();

}
