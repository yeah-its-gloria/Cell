// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/Implementations/PulseAudio.hh>

namespace Cell::Audio::Implementations::PulseAudio {

Wrapped<IRenderer*, Result> Subsystem::CreateRenderer(const Format& format) {
    size_t sampleSize;
    pa_sample_format_t pulseFormat;

    switch (format.type) {
    case FormatType::Float32PCM: {
        sampleSize = (32 * format.channels) / 8;
        pulseFormat = PA_SAMPLE_FLOAT32LE;
        break;
    }

    case FormatType::S16PCM: {
        sampleSize = (16 * format.channels) / 8;
        pulseFormat = PA_SAMPLE_S16LE;
        break;
    }

    default: {
        CELL_UNREACHABLE;
    }
    }

    const pa_sample_spec spec = {
        .format = pulseFormat,
        .rate = format.rate,
        .channels = format.channels
    };

    pa_stream* stream = pa_stream_new(this->context, "Cell", &spec, nullptr);
    if (stream == nullptr) {
        switch (-pa_context_errno(this->context)) {
        default: {
            System::Panic("pa_stream_new failed");
        }
        }
    }

    Renderer* renderer = new Renderer(this, stream, sampleSize);

    pa_stream_set_state_callback(renderer->stream, &Renderer::StateCallback, renderer);
    pa_stream_set_latency_update_callback(renderer->stream, &Renderer::LatencyCallback, renderer);

    int result = -pa_stream_connect_playback(renderer->stream,
                                            nullptr,
                                            nullptr,
                                            (pa_stream_flags)(PA_STREAM_INTERPOLATE_TIMING | PA_STREAM_NOT_MONOTONIC | PA_STREAM_AUTO_TIMING_UPDATE | PA_STREAM_START_CORKED),
                                            nullptr,
                                            nullptr);
    switch (result) {
    case PA_OK: {
        break;
    }

    default: {
        System::Panic("pa_stream_connect_playback failed");
    }
    }

    while (renderer->state != PA_STREAM_READY) {
        switch (renderer->state) {
        case PA_STREAM_UNCONNECTED:
        case PA_STREAM_CREATING: {
            pa_threaded_mainloop_wait(loop);
            break;
        }

        case PA_STREAM_READY: {
            break;
        }

        default: {
            switch (-pa_context_errno(context)) {
            default: {
                System::Panic("pa_stream_connect_playback failed during loop");
            }
            }
        }
        }
    }

    return renderer;
}

}
