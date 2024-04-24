// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Audio/Capturer.hh>
#include <Cell/Audio/Renderer.hh>

#include <AudioToolbox/AudioQueue.h>
#include <CoreAudio/CoreAudio.h>

namespace Cell::Audio::Implementations::CoreAudio {

// CoreAudio subsystem implementation.
class Subsystem : public ISubsystem {
friend class Renderer;

public:
    CELL_FUNCTION static Wrapped<Subsystem*, Result> New(const String& title);
    CELL_FUNCTION ~Subsystem();

    CELL_FUNCTION Wrapped<IRenderer*, Result> CreateRenderer(const Format& format) override;

    CELL_FUNCTION Wrapped<IRenderer*, Result> CreateRenderer(const DeviceInfo& info, const Format& format) override;
    CELL_FUNCTION Wrapped<ICapturer*, Result> CreateLoopback(const DeviceInfo& info, const Format& format) override;
    CELL_FUNCTION Wrapped<Collection::List<DeviceInfo>, Result> DiscoverAvailableRenderers() override;

private:
    CELL_FUNCTION_INTERNAL Subsystem() { };
};

// CoreAudio renderer implementation.
class Renderer : public IRenderer {
friend Subsystem;

public:
    CELL_FUNCTION ~Renderer();

    CELL_FUNCTION Result Start() override;
    CELL_FUNCTION Result Stop() override;
    CELL_FUNCTION Result Submit(const Memory::IBlock& block) override;
    CELL_FUNCTION Wrapped<uint32_t, Result> GetMaxSampleCount() override;
    CELL_FUNCTION Wrapped<uint32_t, Result> GetCurrentSampleOffset() override;
    CELL_FUNCTION uint32_t GetLatency() override;

private:
    CELL_FUNCTION_INTERNAL Renderer(Subsystem* CELL_NONNULL s, AudioDeviceID d) : subsystem(s), device(d), queue(nullptr), buffer(nullptr) { }

    CELL_FUNCTION_INTERNAL static void OutputBufferReadyCallback(void* CELL_NULLABLE data, AudioQueueRef queue, AudioQueueBufferRef buffer);

    Subsystem* subsystem;

    AudioDeviceID device;
    AudioQueueRef queue;

    AudioQueueBufferRef buffer;
};

class Capturer : public ICapturer {
friend Subsystem;

public:
    CELL_FUNCTION ~Capturer();

    CELL_FUNCTION Result Start() override;
    CELL_FUNCTION Result Stop() override;
    CELL_FUNCTION Wrapped<uint32_t, Result> GetAvailableSampleCount() override;
    CELL_FUNCTION Wrapped<uint8_t*, Result> Fetch(const uint32_t count) override;

private:
    CELL_FUNCTION_INTERNAL Capturer(Subsystem* CELL_NONNULL s, AudioDeviceID d) : subsystem(s), device(d), queue(nullptr) { }

    CELL_FUNCTION_INTERNAL static void InputBufferFunc(void* CELL_NULLABLE data, AudioQueueRef queue, AudioQueueBufferRef buffer,
        const AudioTimeStamp* CELL_NONNULL startTime, UInt32 descriptionCount, const AudioStreamPacketDescription* CELL_NULLABLE descriptions);

    Subsystem* subsystem;

    AudioDeviceID device;
    AudioQueueRef queue;
};


}
