// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/Implementations/WASAPI.hh>

namespace Cell::Audio::Implementations::WASAPI {

Wrapped<IRenderer*, Result> Subsystem::CreateRendererImpl(IMMDevice* device, const Format& format) {
    Wrapped<IAudioClient3*, Result> clientResult = this->ActivateClientImpl(device, format, AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM | AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY);
    if (!clientResult.IsValid()) {
        device->Release();
        return clientResult.Result();
    }

    IAudioClient3* client = clientResult.Unwrap();

    IAudioRenderClient* renderer = nullptr;
    const HRESULT result = client->GetService(__uuidof(IAudioRenderClient), (void**)&renderer);
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

    return new Renderer(this, device, client, renderer, GetSampleByteSize(format));
}

Wrapped<IRenderer*, Result> Subsystem::CreateRenderer(const Format& format) {
    Wrapped<IMMDevice*, Result> findDevResult = this->FindDeviceByID(nullptr);
    if (!findDevResult.IsValid()) {
        return findDevResult.Result();
    }

    return this->CreateRendererImpl(findDevResult.Unwrap(), format);
}

Wrapped<IRenderer*, Result> Subsystem::CreateRenderer(const DeviceInfo& info, const Format& format) {
    Wrapped<IMMDevice*, Result> findDevResult = this->FindDeviceByID((LPWSTR)info.identifier);
    if (!findDevResult.IsValid()) {
        return findDevResult.Result();
    }

    return this->CreateRendererImpl(findDevResult.Unwrap(), format);
}

}
