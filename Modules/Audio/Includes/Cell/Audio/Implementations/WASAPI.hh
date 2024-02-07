// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Audio/Renderer.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#include <Audioclient.h>
#include <mmdeviceapi.h>

namespace Cell::Audio::Implementations::WASAPI {

// WASAPI subsystem implementation.
class Subsystem : public ISubsystem {
public:
    CELL_FUNCTION static Wrapped<Subsystem*, Result> New(const System::String& title);
    CELL_FUNCTION ~Subsystem() override;
    CELL_FUNCTION Wrapped<IRenderer*, Result> CreateRenderer(Format format) override;

private:
    CELL_FUNCTION_INTERNAL Subsystem(IMMDeviceEnumerator* CELL_NONNULL e, const System::String& t) : enumerator(e), title(t) { }

    IMMDeviceEnumerator* enumerator;
    System::String title;
};

// WASAPI renderer implementation.
class Renderer : public IRenderer {
friend Subsystem;

public:
    CELL_FUNCTION ~Renderer() override;
    CELL_FUNCTION Result Start() override;
    CELL_FUNCTION Result Stop() override;
    CELL_FUNCTION Result Submit(const IBlock& block) override;
    CELL_FUNCTION Wrapped<uint32_t, Result> GetMaxSampleCount() override;
    CELL_FUNCTION Wrapped<uint32_t, Result> GetCurrentSampleOffset() override;
    CELL_FUNCTION uint32_t GetLatency() override;

private:
    CELL_INLINE Renderer(Subsystem*          CELL_NONNULL s,
                         IMMDevice*          CELL_NONNULL d,
                         IAudioClient3*      CELL_NONNULL c,
                         IAudioRenderClient* CELL_NONNULL rc,
                         size_t                           ss)
        : subsystem(s),
          device(d),
          client(c),
          renderClient(rc),
          sampleSize(ss) { }

    Subsystem* subsystem;

    IMMDevice* device;
    IAudioClient3* client;
    IAudioRenderClient* renderClient;

    size_t sampleSize;

    const uint32_t latency = 1000000000.0;
};

}
