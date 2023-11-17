// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/WASAPI.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Audio {

Result WASAPI::FindRenderDevice() {
    const HRESULT result = this->enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &this->renderDevice);
    switch (result) {
    case S_OK: {
        break;
    }

    case E_NOTFOUND: {
        return Result::RunningOnIncompatibleHardware;
    }

    case E_OUTOFMEMORY: {
        return Result::OutOfMemory;
    }

    default: {
        System::Panic("IMMDeviceEnumerator::GetDefaultAudioEndpoint failed");
    }
    }

    return Result::Success;
}

}
