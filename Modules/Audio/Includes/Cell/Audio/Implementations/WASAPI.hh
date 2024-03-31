// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Audio/Capturer.hh>
#include <Cell/Audio/Renderer.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#include <Audioclient.h>
#include <mmdeviceapi.h>

namespace Cell::Audio::Implementations::WASAPI {

class CellMMNotificationClient;

// WASAPI subsystem implementation.
class Subsystem : public ISubsystem {
public:
    CELL_FUNCTION static Wrapped<Subsystem*, Result> New(const String& title);
    CELL_FUNCTION ~Subsystem();
    CELL_FUNCTION Wrapped<IRenderer*, Result> CreateRenderer(const Format& format) override;
    CELL_FUNCTION Wrapped<IRenderer*, Result> CreateRenderer(const DeviceInfo& info, const Format& format) override;
    CELL_FUNCTION Wrapped<ICapturer*, Result> CreateLoopback(const DeviceInfo& info, const Format& format) override;
    CELL_FUNCTION Wrapped<Collection::List<DeviceInfo>, Result> DiscoverAvailableRenderers() override;

private:
    CELL_FUNCTION_INTERNAL Subsystem(IMMDeviceEnumerator* CELL_NONNULL e, CellMMNotificationClient* CELL_NONNULL n, const String& t)
        : enumerator(e), notificationClient(n), title(t) { }

    CELL_FUNCTION_INTERNAL Wrapped<IMMDevice*, Result> FindDeviceByID(LPWSTR id);
    CELL_FUNCTION_INTERNAL Wrapped<IAudioClient3*, Result> ActivateClientImpl(IMMDevice* device, const Format& format, const uint32_t flags);
    CELL_FUNCTION_INTERNAL Wrapped<IRenderer*, Result> CreateRendererImpl(IMMDevice* device, const Format& format);

    IMMDeviceEnumerator* enumerator;
    CellMMNotificationClient* notificationClient;
    String title;
};

// WASAPI renderer implementation.
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
    CELL_FUNCTION_INTERNAL Renderer(Subsystem*          CELL_NONNULL s,
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
};

// WASAPI capturer implementation.
class Capturer : public ICapturer {
friend Subsystem;

public:
    CELL_FUNCTION ~Capturer();

    CELL_FUNCTION Result Start() override;
    CELL_FUNCTION Result Stop() override;
    CELL_FUNCTION Wrapped<uint32_t, Result> GetAvailableSampleCount() override;
    CELL_FUNCTION Wrapped<uint8_t*, Result> Fetch(const uint32_t count) override;

private:
    CELL_FUNCTION_INTERNAL Capturer(Subsystem*           CELL_NONNULL s,
                                    IMMDevice*           CELL_NONNULL d,
                                    IAudioClient3*       CELL_NONNULL c,
                                    IAudioCaptureClient* CELL_NONNULL cc,
                                    size_t                            ss)
        : subsystem(s),
          device(d),
          client(c),
          captureClient(cc),
          sampleSize(ss) { }

    Subsystem* subsystem;

    IMMDevice* device;
    IAudioClient3* client;
    IAudioCaptureClient* captureClient;

    size_t sampleSize;
};

}
