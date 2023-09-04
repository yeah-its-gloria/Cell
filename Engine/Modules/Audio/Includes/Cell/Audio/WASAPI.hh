// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
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

    CELL_FUNCTION Result SetUpRendering() override;
    CELL_FUNCTION Wrapped<uint32_t, Result> GetMaximumSampleCount() override;
    CELL_FUNCTION Wrapped<uint32_t, Result> GetCurrentSampleOffset() override;
    CELL_FUNCTION Result WriteSamples(const uint8_t* CELL_NONNULL data, const uint32_t count, const bool treatAsSilent = false) override;
    CELL_FUNCTION Result PlaybackBegin() override;
    CELL_FUNCTION Result PlaybackEnd() override;

    CELL_INLINE uint16_t GetSampleSizeInBytes() override { return this->sampleSize; }

    CELL_INLINE uint16_t GetSampleCountPerSecond() override { return this->samplesPerSecond; }

    CELL_INLINE uint32_t GetLatencyMicroseconds() override {
        return (uint32_t)(10000000.0 * this->GetMaximumSampleCount().Unwrap() / this->GetSampleCountPerSecond() / 10000 / 2) * 1000;
    }

private:
    CELL_FUNCTION_INTERNAL WASAPI(IMMDeviceEnumerator* CELL_NONNULL enumerator);

    CELL_FUNCTION_INTERNAL Result FindRenderDevice();
    CELL_FUNCTION_INTERNAL Result CreateRenderClient();

    IMMDeviceEnumerator* enumerator = nullptr;
    IMMDevice* renderDevice = nullptr;
    IAudioClient3* renderClient = nullptr;
    IAudioRenderClient* renderClientService = nullptr;

    uint16_t sampleSize = (2 * 32) / 8; // TODO: store full format info instead // in bytes
    uint16_t samplesPerSecond = 48000;
};

}
