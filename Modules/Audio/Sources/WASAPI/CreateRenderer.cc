// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/Implementations/WASAPI.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Audio::Implementations::WASAPI {

Wrapped<IRenderer*, Result> Subsystem::CreateRenderer(Format format) {
    IMMDevice* device = nullptr;
    HRESULT result = this->enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
    switch (result) {
    case S_OK: {
        break;
    }

    case E_NOTFOUND: {
        return Result::ServiceUnavailable;
    }

    case E_OUTOFMEMORY: {
        return Result::NotEnoughMemory;
    }

    default: {
        System::Panic("IMMDeviceEnumerator::GetDefaultAudioEndpoint failed");
    }
    }

    GUID formatType;
    uint16_t bitCount;
    switch (format.type) {
    case FormatType::Float32PCM: {
        formatType = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;
        bitCount = 32;
        break;
    }

    case FormatType::S16PCM: {
        formatType = KSDATAFORMAT_SUBTYPE_PCM;
        bitCount = 16;
        break;
    }

    default:  {
        CELL_UNREACHABLE;
    }
    }

    const WAVEFORMATEXTENSIBLE exFormat = {
        .Format = {
            .wFormatTag = WAVE_FORMAT_EXTENSIBLE,
            .nChannels = format.channels,
            .nSamplesPerSec = format.rate,
            .nAvgBytesPerSec = format.rate * ((format.channels * bitCount) / 8),
            .nBlockAlign = (WORD)((format.channels * bitCount) / 8),
            .wBitsPerSample = bitCount,
            .cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX)
        },

        .Samples.wValidBitsPerSample = bitCount,
        .dwChannelMask = KSAUDIO_SPEAKER_STEREO,
        .SubFormat = formatType,
    };

    IAudioClient3* client = nullptr;
    result = device->Activate(__uuidof(IAudioClient3), CLSCTX_INPROC_SERVER, nullptr, (void**)&client);
    switch (result) {
    case S_OK: {
        break;
    }

    case E_NOTFOUND:
    case E_NOINTERFACE: {
        device->Release();
        return Result::ServiceUnavailable;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED: {
        device->Release();
        return Result::TargetLost;
    }

    case E_OUTOFMEMORY: {
        device->Release();
        return Result::NotEnoughMemory;
    }

    default: {
        System::Panic("IMMDevice::Activate failed");
    }
    }

    result = client->Initialize(AUDCLNT_SHAREMODE_SHARED,
                                AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM | AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY,
                                10000000,
                                0,
                                (const WAVEFORMATEX*)&exFormat,
                                nullptr
    );

    switch (result) {
    case S_OK: {
        break;
    }

    case E_INVALIDARG: {
        client->Release();
        device->Release();
        return Result::InvalidParameters;
    }

    case E_NOTFOUND:
    case AUDCLNT_E_SERVICE_NOT_RUNNING: {
        client->Release();
        device->Release();
        return Result::ServiceUnavailable;
    }

    case AUDCLNT_E_UNSUPPORTED_FORMAT: {
        client->Release();
        device->Release();
        return Result::UnsupportedFormat;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED:
    case AUDCLNT_E_ENDPOINT_CREATE_FAILED: {
        client->Release();
        device->Release();
        return Result::TargetLost;
    }

    case AUDCLNT_E_DEVICE_IN_USE:
    case AUDCLNT_E_EXCLUSIVE_MODE_NOT_ALLOWED: {
        client->Release();
        device->Release();
        return Result::TargetOccupied;
    }

    case E_OUTOFMEMORY: {
        client->Release();
        device->Release();
        return Result::NotEnoughMemory;
    }

    default: {
        System::Panic("IAudioClient3::Initialize failed");
    }
    }

    IAudioRenderClient* renderer = nullptr;
    result = client->GetService(__uuidof(IAudioRenderClient), (void**)&renderer);
    switch (result) {
    case S_OK: {
        break;
    }

    case E_NOINTERFACE:
    case AUDCLNT_E_SERVICE_NOT_RUNNING: {
        client->Release();
        device->Release();
        return Result::ServiceUnavailable;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED: {
        client->Release();
        device->Release();
        return Result::TargetLost;
    }

    default: {
        System::Panic("IAudioClient3::GetService failed");
    }
    }

    return new Renderer(this, device, client, renderer, (format.channels * bitCount) / 8);
}

}
