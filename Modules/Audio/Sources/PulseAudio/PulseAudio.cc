// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/PulseAudio.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Audio {

PulseAudio::~PulseAudio() {
    pa_simple_free(this->engine);
}

Wrapped <uint32_t, Result> PulseAudio::GetBufferSize() {
    const uint32_t count = 48000; // TODO: figure this out properly

    return count;
}

Result PulseAudio::WriteSamples(const uint8_t* data, const uint32_t count, const bool treatAsSilent) {
    (void)(treatAsSilent);

    int error = 0;
    int result = pa_simple_write(this->engine, data, count * ((32 / 8) * 2), &error);
    CELL_ASSERT(result >= 0);

    return Result::Success;
}

Result PulseAudio::PlaybackBegin() {
    return Result::Success; // TODO: figure out how to do this better for PulseAudio
}

Result PulseAudio::PlaybackEnd() {
    int error = 0;
    int result = pa_simple_drain(this->engine, &error);
    CELL_ASSERT(result >= 0);

    return Result::Success; // TODO: figure out how to do this better for PulseAudio
}

Wrapped <uint32_t, Result> PulseAudio::GetCurrentBufferFillCount() {
    const uint32_t offset = 0; // TODO: figure this out properly

    return offset;
}

uint32_t PulseAudio::GetLatencyMicroseconds() {
    int error = 0;
    pa_usec_t latency = pa_simple_get_latency(this->engine, &error);
    CELL_ASSERT(latency != (pa_usec_t) - 1);

    return (uint32_t)latency;
}

}
