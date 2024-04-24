// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/Implementations/CoreAudio.hh>
#include <Cell/System/Log.hh>

namespace Cell::Audio::Implementations::CoreAudio {

Renderer::~Renderer() {
    CELL_ASSERT(this->buffer != nullptr);

    AudioQueueFreeBuffer(this->queue, this->buffer);
    AudioQueueDispose(this->queue, TRUE);
}

Result Renderer::Start() {
    const OSStatus result = AudioQueueStart(this->queue, nullptr);
    CELL_ASSERT(result == kAudioHardwareNoError);

    return Result::Success;
}

Result Renderer::Stop() {
    const OSStatus result = AudioQueueStop(this->queue, TRUE);
    CELL_ASSERT(result == kAudioHardwareNoError);

    return Result::Success;
}

Result Renderer::Submit(const Memory::IBlock& block) {
    if (this->buffer == nullptr) {
        System::Log("BUFFER MISS!!!");
        return Result::Success;
    }

    CELL_ASSERT(this->buffer->mAudioDataBytesCapacity > block.GetSize());

    AudioQueueBufferRef buffer = this->buffer;
    this->buffer = nullptr;

    Memory::Copy((uint8_t*)buffer->mAudioData, block.AsBytes(), block.GetSize());
    buffer->mAudioDataByteSize = block.GetSize();

    const OSStatus result = AudioQueueEnqueueBuffer(this->queue, buffer, 0, nullptr);
    CELL_ASSERT(result == kAudioHardwareNoError);

    return Result::Success;
}

Wrapped<uint32_t, Result> Renderer::GetCurrentSampleOffset() {
    return 0;
}

uint32_t Renderer::GetLatency() {
    return 20000;
}

Wrapped<uint32_t, Result> Renderer::GetMaxSampleCount() {
    return 48000;
}

void Renderer::OutputBufferReadyCallback(void* data, AudioQueueRef queue, AudioQueueBufferRef buffer) {
    Renderer* renderer = (Renderer*)data;

    CELL_ASSERT(renderer != nullptr);
    CELL_ASSERT(renderer->queue == queue);
    CELL_ASSERT(renderer->buffer == nullptr);

    renderer->buffer = buffer;
}

}
