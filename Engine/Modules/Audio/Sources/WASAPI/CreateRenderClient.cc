// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/WASAPI.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Audio {

Result WASAPI::CreateRenderClient() {
    if (this->renderDevice == nullptr) {
        return Result::InvalidState;
    }

    HRESULT result = this->renderDevice->Activate(__uuidof(IAudioClient3), CLSCTX_INPROC_SERVER, nullptr, (void**)&this->renderClient);
    switch (result) {
    case S_OK: {
        break;
    }

    case E_NOTFOUND: {
        return Result::RunningOnIncompatibleHardware;
    }

    case E_NOINTERFACE: {
        return Result::RunningOnIncompatibleSoftware;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED: {
        return Result::DeviceLost;
    }

    case E_OUTOFMEMORY: {
        return Result::OutOfMemory;
    }

    default: {
        System::Panic("IMMDevice::Activate failed");
    }
    }

    const WAVEFORMATEXTENSIBLE format = {
        .Format = {
            .wFormatTag = WAVE_FORMAT_EXTENSIBLE,
            .nChannels = 2,
            .nSamplesPerSec = 48000, // in Hertz
            .nAvgBytesPerSec = 48000 * ((2 * 32) / 8), // freq * block
            .nBlockAlign = (2 * 32) / 8, // channel * bits per sample, in bits
            .wBitsPerSample = 32,
            .cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX)
        },

        .Samples.wValidBitsPerSample = 32,
        .dwChannelMask = KSAUDIO_SPEAKER_STEREO,
        .SubFormat = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT,
    };

    result = this->renderClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
                                            AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM | AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY,
                                            10000000,
                                            0,
                                            (const WAVEFORMATEX*)&format,
                                            nullptr
    );

    switch (result) {
    case S_OK: {
        break;
    }

    case E_INVALIDARG: {
        this->renderClient->Release();
        return Result::InvalidParameters;
    }

    case E_NOTFOUND:
    case AUDCLNT_E_INVALID_DEVICE_PERIOD: {
        this->renderClient->Release();
        return Result::RunningOnIncompatibleHardware;
    }

    case AUDCLNT_E_SERVICE_NOT_RUNNING: {
        this->renderClient->Release();
        return Result::RunningOnIncompatibleSoftware;
    }

    case AUDCLNT_E_UNSUPPORTED_FORMAT: {
        this->renderClient->Release();
        return Result::UnsupportedFormat;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED:
    case AUDCLNT_E_ENDPOINT_CREATE_FAILED: {
        this->renderClient->Release();
        return Result::DeviceLost;
    }

    case AUDCLNT_E_DEVICE_IN_USE:
    case AUDCLNT_E_EXCLUSIVE_MODE_NOT_ALLOWED: {
        this->renderClient->Release();
        return Result::DeviceOccupied;
    }

    case E_OUTOFMEMORY: {
        this->renderClient->Release();
        return Result::OutOfMemory;
    }

    default: {
        System::Panic("IAudioClient3::Initialize failed");
    }
    }

    result = this->renderClient->GetService(__uuidof(IAudioRenderClient), (void**)&this->renderClientService);
    switch (result) {
    case S_OK: {
        break;
    }

    case E_NOINTERFACE:
    case AUDCLNT_E_SERVICE_NOT_RUNNING: {
        return Result::RunningOnIncompatibleSoftware;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED: {
        return Result::DeviceLost;
    }

    default: {
        System::Panic("IAudioClient3::GetService failed");
    }
    }

    return Result::Success;
}

}
