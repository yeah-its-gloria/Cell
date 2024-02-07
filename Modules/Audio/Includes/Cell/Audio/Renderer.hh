// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Audio/Subsystem.hh>
#include <Cell/System/Block.hh>

namespace Cell::Audio {

// Base audio renderer interface.
class IRenderer : public Object {
public:
    // Common destructor.
    virtual ~IRenderer() = default;

    // Starts playback.
    virtual Result Start() = 0;

    // Stops playback.
    virtual Result Stop() = 0;

    // Submits the given raw samples to the stream.
    virtual Result Submit(const IBlock& block) = 0;

    // Returns the maximum number of samples that can be buffered.
    virtual Wrapped<uint32_t, Result> GetMaxSampleCount() = 0;

    // Returns the current offset into the stored buffer.
    virtual Wrapped<uint32_t, Result> GetCurrentSampleOffset() = 0;

    // Returns the playback latency in microseconds.
    virtual uint32_t GetLatency() = 0;
};

}
