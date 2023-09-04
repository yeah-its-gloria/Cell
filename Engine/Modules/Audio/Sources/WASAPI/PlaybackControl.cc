// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/WASAPI.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Audio {

Result WASAPI::PlaybackBegin() {
    const HRESULT result = this->renderClient->Start();
    switch (result) {
    case S_OK: {
        break;
    }

    case AUDCLNT_E_NOT_STOPPED: {
        return Result::AlreadyInRequestedState;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED: {
        return Result::DeviceLost;
    }

    default: {
        System::Panic("IAudioClient3::Start failed");
    }
    }

    return Result::Success;
}

Result WASAPI::PlaybackEnd() {
    const HRESULT result = this->renderClient->Start();
    switch (result) {
    case S_OK: {
        break;
    }

    case S_FALSE: {
        return Result::AlreadyInRequestedState;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED: {
        return Result::DeviceLost;
    }

    case AUDCLNT_E_NOT_STOPPED: {
        return Result::NotYetFinished;
    }

    default: {
        System::Panic("IAudioClient3::Stop failed");
    }
    }

    return Result::Success;
}

}
