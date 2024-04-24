// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/Implementations/CoreAudio.hh>

namespace Cell::Audio::Implementations::CoreAudio {

Capturer::~Capturer() {
    AudioQueueDispose(this->queue, TRUE);
}

Result Capturer::Start() {
    AudioQueueStart(this->queue, nullptr);
    return Result::Success;
}

Result Capturer::Stop() {
    AudioQueueStop(this->queue, TRUE);
    return Result::Success;
}

Wrapped<uint32_t, Result> Capturer::GetAvailableSampleCount() {
    return 0;
}

Wrapped<uint8_t*, Result> Capturer::Fetch(const uint32_t count) {
    (void)(count);

    return Result::NoMoreSamples;
}

void Capturer::InputBufferFunc(void* data, AudioQueueRef queue, AudioQueueBufferRef buffer,
    const AudioTimeStamp* startTime, UInt32 descriptionCount, const AudioStreamPacketDescription* descriptions) {
    (void)(buffer);
    (void)(startTime);
    (void)(descriptionCount);
    (void)(descriptions);

    Capturer* capturer = (Capturer*)data;

    CELL_ASSERT(capturer != nullptr);
    CELL_ASSERT(capturer->queue == queue);
}

}
