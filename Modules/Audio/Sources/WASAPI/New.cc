// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/WASAPI.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Audio {

Wrapped<WASAPI*, Result> WASAPI::New() {
    IMMDeviceEnumerator* enumerator = nullptr;
    const HRESULT result = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (void**)&enumerator);
    switch (result) {
    case S_OK: {
        break;
    }

    case CO_E_NOTINITIALIZED: {
        return Result::PlatformNotReadyYet;
    }

    case REGDB_E_CLASSNOTREG:
    case E_NOINTERFACE: {
        return Result::RunningOnIncompatibleHardware;
    }

    default: {
        System::Panic("CoCreateInstance failed");
    }
    }

    return new WASAPI(enumerator);
}

}
