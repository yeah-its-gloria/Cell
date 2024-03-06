// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>

#include "NotificationImpl.hh"

Cell::Audio::DeviceInfo::~DeviceInfo() {
    CoTaskMemFree(this->identifier);
}

namespace Cell::Audio::Implementations::WASAPI {

struct FormatInfo {
    GUID type;
    uint16_t bits;
    uint32_t mask;
};

CELL_FUNCTION_INTERNAL CELL_INLINE constexpr uint32_t GetChannelMask(uint8_t channels) {
    switch (channels) {
    case 1: {
        return KSAUDIO_SPEAKER_MONO;
    }

    case 2: {
        return KSAUDIO_SPEAKER_STEREO;
    }

    case 4: {
        return KSAUDIO_SPEAKER_SURROUND;
    }

    default: {
        break;
    }
    }

    CELL_UNREACHABLE;
}

Wrapped<Subsystem*, Result> Subsystem::New(const String& title) {
    if (title.IsEmpty()) {
        return Result::InvalidParameters;
    }

    IMMDeviceEnumerator* enumerator = nullptr;
    const HRESULT result = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (void**)&enumerator);
    switch (result) {
    case S_OK: {
        break;
    }

    case CO_E_NOTINITIALIZED:
    case REGDB_E_CLASSNOTREG:
    case E_NOINTERFACE: {
        return Result::ServiceUnavailable;
    }

    default: {
        System::Panic("CoCreateInstance failed");
    }
    }

    return new Subsystem(enumerator, new CellMMNotificationClient(enumerator), title);
}

Subsystem::~Subsystem() {
    this->notificationClient->Release();
    this->enumerator->Release();
}

Wrapped<IAudioClient3*, Result> Subsystem::ActivateClientImpl(IMMDevice* device, const Format& format, const uint32_t flags) {
    switch (format.channels) {
    case 1:
    case 2:
    case 4: {
        break;
    }

    default: {
        return Result::UnsupportedFormat;
    }
    }

    const FormatInfo info = [&format]{
        switch (format.type) {
        case FormatType::Float32PCM: {
            return FormatInfo { KSDATAFORMAT_SUBTYPE_IEEE_FLOAT, 32, GetChannelMask(format.channels) };
        }

        case FormatType::S16PCM: {
            return FormatInfo { KSDATAFORMAT_SUBTYPE_PCM, 16, GetChannelMask(format.channels) };
        }

        default:  {
            CELL_UNREACHABLE;
        }
        }
    }();

    const WAVEFORMATEXTENSIBLE exFormat = {
        .Format = {
            .wFormatTag = WAVE_FORMAT_EXTENSIBLE,
            .nChannels = format.channels,
            .nSamplesPerSec = format.rate,
            .nAvgBytesPerSec = format.rate * ((format.channels * info.bits) / 8),
            .nBlockAlign = (WORD)((format.channels * info.bits) / 8),
            .wBitsPerSample = info.bits,
            .cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX)
        },

        .Samples.wValidBitsPerSample = info.bits,
        .dwChannelMask = info.mask,
        .SubFormat = info.type,
    };

    IAudioClient3* client = nullptr;
    HRESULT result = device->Activate(__uuidof(IAudioClient3), CLSCTX_INPROC_SERVER, nullptr, (void**)&client);
    switch (result) {
    case S_OK: {
        break;
    }

    case E_NOTFOUND:
    case E_NOINTERFACE: {
        return Result::ServiceUnavailable;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED: {
        return Result::TargetLost;
    }

    case E_OUTOFMEMORY: {
        return Result::NotEnoughMemory;
    }

    default: {
        System::Panic("IMMDevice::Activate failed");
    }
    }

    result = client->Initialize(AUDCLNT_SHAREMODE_SHARED, flags, 10000000, 0, (const WAVEFORMATEX*)&exFormat, nullptr);
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

    return client;
}

}
