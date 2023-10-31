// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Audio/Engine.hh>

#include <pulse/pulseaudio.h>
#include <pulse/simple.h>

namespace Cell::Audio {

// PulseAudio implementation.
class PulseAudio : public IEngine {
public:
    CELL_FUNCTION static Wrapped<PulseAudio*, Result> New();
    CELL_FUNCTION ~PulseAudio() override;

    CELL_FUNCTION Result SetUpRendering(Format format) override;
    CELL_FUNCTION Result WriteSamples(const uint8_t* CELL_NONNULL data, const uint32_t count, const bool treatAsSilent = false) override;
    CELL_FUNCTION Result PlaybackBegin() override;
    CELL_FUNCTION Result PlaybackEnd() override;
    CELL_FUNCTION Wrapped <uint32_t, Result> GetBufferSize() override;
    CELL_FUNCTION Wrapped <uint32_t, Result> GetCurrentBufferFillCount() override;
    CELL_FUNCTION uint32_t GetLatencyMicroseconds() override;

private:
    CELL_FUNCTION_INTERNAL PulseAudio(pa_simple* engine) : engine(engine) { };

    pa_simple* engine;
};

}
