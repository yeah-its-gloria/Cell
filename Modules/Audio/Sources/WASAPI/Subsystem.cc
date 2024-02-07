// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Audio/Implementations/WASAPI.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Audio::Implementations::WASAPI {

Wrapped<Subsystem*, Result> Subsystem::New(const System::String& title) {
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

    return new Subsystem(enumerator, title);
}

Subsystem::~Subsystem() {
    this->enumerator->Release();
}

}
