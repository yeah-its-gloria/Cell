// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/PulseAudio.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Audio {

Wrapped<PulseAudio*, Result> PulseAudio::New() {
    static const pa_sample_spec sampleSpec = {
        .format   = PA_SAMPLE_FLOAT32LE,
        .rate     = 48000,
        .channels = 2
    };

    int error = 0;
    pa_simple* engine = pa_simple_new(nullptr, "Cell", PA_STREAM_PLAYBACK, nullptr, "Playback", &sampleSpec, nullptr, nullptr, &error);
    CELL_ASSERT(engine != nullptr);

    return new PulseAudio(engine);
}

}
