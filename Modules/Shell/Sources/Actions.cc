// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Shell.hh>

namespace Cell::Shell {

Result IShell::RegisterAction(const KeyboardButton button, const ButtonFunction function, void* userData) {
    const RegisterInfo info = {
        .type = 0,
        .userData = userData,
        .keyboard = button,
        .button = function
    };

    this->registeredFunctions.Append(info);
    return Result::Success;
}

Result IShell::RegisterAction(const MouseButton button, const ButtonFunction function, void* userData) {
    const RegisterInfo info = {
        .type = 1,
        .userData = userData,
        .mouse = button,
        .button = function
    };

    this->registeredFunctions.Append(info);
    return Result::Success;
}

Result IShell::RegisterAction(const ControllerButton button, const ButtonFunction function, void* userData) {
    const RegisterInfo info = {
        .type = 2,
        .userData = userData,
        .controller = button,
        .button = function
    };

    this->registeredFunctions.Append(info);
    return Result::Success;
}

Result IShell::RegisterAction(const MouseAxis axis, const AxisFunction function, void* userData) {
    const RegisterInfo info = {
        .type = 3,
        .userData = userData,
        .mouseAxis = axis,
        .axis = function
    };

    this->registeredFunctions.Append(info);
    return Result::Success;
}

Result IShell::RegisterAction(const ControllerAxis axis, const AxisFunction function, void* userData) {
    const RegisterInfo info = {
        .type = 4,
        .userData = userData,
        .controllerAxis = axis,
        .axis = function
    };

    this->registeredFunctions.Append(info);
    return Result::Success;
}

}
