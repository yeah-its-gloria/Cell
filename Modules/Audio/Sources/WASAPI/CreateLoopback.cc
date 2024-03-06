// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/Implementations/WASAPI.hh>

namespace Cell::Audio::Implementations::WASAPI {

Wrapped<ICapturer*, Result> Subsystem::CreateLoopback(const DeviceInfo& info, const Format& format) {
    Wrapped<IMMDevice*, Result> findDevResult = this->FindDeviceByID((LPWSTR)info.identifier);
    if (!findDevResult.IsValid()) {
        return findDevResult.Result();
    }

    IMMDevice* device = findDevResult.Unwrap();

    Wrapped<IAudioClient3*, Result> clientResult = this->ActivateClientImpl(device, format, AUDCLNT_STREAMFLAGS_LOOPBACK | AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM | AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY);
    if (!clientResult.IsValid()) {
        device->Release();
        return clientResult.Result();
    }

    IAudioClient3* client = clientResult.Unwrap();

    IAudioCaptureClient* capturer = nullptr;
    const HRESULT result = client->GetService(__uuidof(IAudioCaptureClient), (void**)&capturer);
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

    return new Capturer(this, device, client, capturer, GetSampleByteSize(format));
}

}
