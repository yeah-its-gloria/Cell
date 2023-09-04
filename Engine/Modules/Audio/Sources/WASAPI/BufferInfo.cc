// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/WASAPI.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Audio {

Wrapped<uint32_t, Result> WASAPI::GetMaximumSampleCount() {
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

Wrapped<uint32_t, Result> WASAPI::GetCurrentSampleOffset() {
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
