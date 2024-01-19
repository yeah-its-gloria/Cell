// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/WASAPI.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Audio {

WASAPI::WASAPI(IMMDeviceEnumerator* enumerator) {
    this->enumerator = enumerator;
}

WASAPI::~WASAPI() {
    if (this->renderClientService != nullptr) {
        this->renderClientService->Release();
    }

    if (this->renderClient != nullptr) {
        this->renderClient->Release();
    }

    if (this->renderDevice != nullptr) {
        this->renderDevice->Release();
    }

    this->enumerator->Release();
}

Wrapped<uint32_t, Result> WASAPI::GetBufferSize() {
    if (this->renderClient == NULL) {
        return Result::InvalidState;
    }

    uint32_t count = 0;
    const HRESULT result = this->renderClient->GetBufferSize(&count);
    switch (result) {
    case S_OK: {
        break;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED: {
        return Result::DeviceLost;
    }

    default: {
        System::Panic("IAudioClient3::GetBufferSize failed");
    }
    }

    return count;
}

Wrapped<uint32_t, Result> WASAPI::GetCurrentBufferFillCount() {
    if (this->renderClient == NULL) {
        return Result::InvalidState;
    }

    uint32_t offset = 0;
    const HRESULT result = this->renderClient->GetCurrentPadding(&offset);
    switch (result) {
    case S_OK: {
        break;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED: {
        return Result::DeviceLost;
    }

    default: {
        System::Panic("IAudioClient3::GetCurrentPadding failed");
    }
    }

    return offset;
}
}
