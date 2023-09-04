// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Input.hh>
#include <Cell/System/Platform/Linux.hh>

namespace Cell::Shell {

Result Input::RegisterAction(const KeyValue value, const InputFunction function, void* userData) {
    System::Platform::Linux* _linux = (System::Platform::Linux*)&this->platform;
    if (_linux->keyboardState == nullptr) {
        return Result::PlatformNotReadyYet;
    }

    const registerInfo info =
        {
            .match    = value,
            .function = function,
            .userData = userData
        };

    this->registeredFunctions.Append(info);

    return Result::Success;
}

Result Input::Poll() {
    System::Platform::Linux* _linux = (System::Platform::Linux*)&this->platform;
    if (_linux->keyboardState == nullptr) {
        return Result::PlatformNotReadyYet;
    }

    for (registerInfo info : this->registeredFunctions) {
        if ((_linux->keys & info.match) == info.match) {
            info.function(info.match, info.userData);
        }
    }

    return Result::Success;
}

}
