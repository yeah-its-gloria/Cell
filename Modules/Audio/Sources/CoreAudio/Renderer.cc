// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/Implementations/CoreAudio.hh>

namespace Cell::Audio::Implementations::CoreAudio {

Renderer::~Renderer() {
}

Result Renderer::Start() {
    AudioQueueStart(this->queue, nullptr);
    return Result::Success;
}

Result Renderer::Stop() {
    AudioQueueStop(this->queue, TRUE);
    return Result::Success;
}

Result Renderer::Submit(const Memory::IBlock& block) {
    (void)(block);

    // TODO: implement

    return Result::Success;
}

Wrapped<uint32_t, Result> Renderer::GetCurrentSampleOffset() {
    return 0;
}

uint32_t Renderer::GetLatency() {
    return 1;
}

Wrapped<uint32_t, Result> Renderer::GetMaxSampleCount() {
    return 48000;
}

void Renderer::OutputBufferFunc(void* data, AudioQueueRef queue, AudioQueueBufferRef buffer) {
    (void)(buffer);

    Renderer* renderer = (Renderer*)data;
    CELL_ASSERT(renderer != nullptr);
    CELL_ASSERT(renderer->queue == queue);
}

}
