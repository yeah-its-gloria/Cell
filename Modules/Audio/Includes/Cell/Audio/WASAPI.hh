// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Audio/Engine.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#include <Audioclient.h>
#include <mmdeviceapi.h>

namespace Cell::Audio {

// Windows Audio Session API implementation
class WASAPI : public IEngine {
public:
    CELL_FUNCTION static Wrapped<WASAPI*, Result> New();
    CELL_FUNCTION ~WASAPI() override;

    CELL_FUNCTION Result SetUpRendering(Format format) override;
    CELL_FUNCTION Result WriteSamples(const uint8_t* CELL_NONNULL data, const uint32_t count, const bool treatAsSilent = false) override;
    CELL_FUNCTION Result PlaybackBegin() override;
    CELL_FUNCTION Result PlaybackEnd() override;
    CELL_FUNCTION Wrapped<uint32_t, Result> GetBufferSize() override;
    CELL_FUNCTION Wrapped<uint32_t, Result> GetCurrentBufferFillCount() override;

    CELL_INLINE uint32_t GetLatencyMicroseconds() override {
        return 1000000000.0;
    }

private:
    CELL_FUNCTION_INTERNAL WASAPI(IMMDeviceEnumerator* CELL_NONNULL e) : enumerator(e) { }

    CELL_FUNCTION_INTERNAL Result FindRenderDevice();
    CELL_FUNCTION_INTERNAL Result CreateRenderClient(Format format);

    IMMDeviceEnumerator* enumerator;
    IMMDevice* renderDevice = nullptr;
    IAudioClient3* renderClient = nullptr;
    IAudioRenderClient* renderClientService = nullptr;

    uint32_t sampleSize = 0;
};

}
