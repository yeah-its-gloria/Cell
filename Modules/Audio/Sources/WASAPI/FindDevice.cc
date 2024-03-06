// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/Implementations/WASAPI.hh>

namespace Cell::Audio::Implementations::WASAPI {

Wrapped<IMMDevice*, Result> Subsystem::FindDeviceByID(LPWSTR id) {
    IMMDevice* device = nullptr;
    HRESULT result = S_OK;

    if (id != nullptr) {
        result = this->enumerator->GetDevice(id, &device);
    } else {
        result = this->enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
    }

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
        if (id != nullptr) { // my god
            System::Panic("IMMDeviceEnumerator::GetDevice failed");
        } else {
            System::Panic("IMMDeviceEnumerator::GetDefaultAudioEndpoint failed");
        }
    }
    }

    return device;
}

}
