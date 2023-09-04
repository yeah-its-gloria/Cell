// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
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

}
