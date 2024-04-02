// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/Implementations/PulseAudio.hh>

namespace Cell::Audio::Implementations::PulseAudio {

// TODO: inform the client that the target stream device might've been lost

Renderer::~Renderer() {
    pa_stream_disconnect(this->stream);
    pa_stream_unref(this->stream);
}

Result Renderer::Start() {
    pa_operation* op = pa_stream_cork(this->stream, 0, nullptr, nullptr);
    CELL_ASSERT(op != nullptr);

    while (pa_operation_get_state(op) == PA_OPERATION_RUNNING) {
        pa_threaded_mainloop_wait(this->subsystem->loop);
    }

    pa_operation_unref(op);
    return Result::Success;
}

Result Renderer::Stop() {
    pa_operation* op = pa_stream_drain(this->stream, nullptr, nullptr);
    CELL_ASSERT(op != nullptr);

    while (pa_operation_get_state(op) == PA_OPERATION_RUNNING) {
        pa_threaded_mainloop_wait(this->subsystem->loop);
    }

    pa_operation_unref(op);

    op = pa_stream_cork(this->stream, 1, nullptr, nullptr);
    CELL_ASSERT(op != nullptr);

    while (pa_operation_get_state(op) == PA_OPERATION_RUNNING) {
        pa_threaded_mainloop_wait(this->subsystem->loop);
    }

    pa_operation_unref(op);
    return Result::Success;
}

Result Renderer::Submit(const Memory::IBlock& block) {
    const size_t size = block.GetSize();
    if (size % this->sampleSize != 0) {
        return Result::InvalidParameters;
    }

    const int result = pa_stream_write(this->stream, block.AsPointer(), size, nullptr, 0, PA_SEEK_RELATIVE);
    switch (result) {
    case PA_OK: {
        break;
    }

    default: {
        System::Panic("pa_stream_write failed");
    }
    }

    return Result::Success;
}

Wrapped<uint32_t, Result> Renderer::GetMaxSampleCount() {
    const pa_buffer_attr* attributes = pa_stream_get_buffer_attr(this->stream);
    CELL_ASSERT(attributes != nullptr);

    return attributes->tlength;
}

Wrapped<uint32_t, Result> Renderer::GetCurrentSampleOffset() {
    Wrapped<uint32_t, Result> result = this->GetMaxSampleCount();
    if (!result.IsValid()) {
        return result.Result();
    }

    const size_t size = pa_stream_writable_size(this->stream);
    if (size == (size_t)-1) {
        return Result::TargetLost;
    }

    return result.Unwrap() - (uint32_t)size;
}

uint32_t Renderer::GetLatency() {
    return (uint32_t)this->latency;
}

void Renderer::StateCallback(pa_stream* stream, void* userdata) {
    Renderer* renderer = (Renderer*)userdata;
    CELL_ASSERT(renderer != nullptr && renderer->stream == stream);

    renderer->state = pa_stream_get_state(renderer->stream);
}

void Renderer::LatencyCallback(pa_stream* stream, void* userdata) {
    Renderer* renderer = (Renderer*)userdata;
    CELL_ASSERT(renderer != nullptr && renderer->stream == stream);

    const int result = pa_stream_get_latency(renderer->stream, &renderer->latency, nullptr);
    switch (result) {
    case PA_OK: {
        break;
    }

    default: {
        System::Panic("pa_stream_get_latency failed");
    }
    }
}

}
