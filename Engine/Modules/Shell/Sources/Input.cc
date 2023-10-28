// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Input.hh>

namespace Cell::Shell {

Result Input::RegisterAction(const KeyboardButton value, const InputFunction function, void* userData) {
    const RegisterInfo info = {
        .match = value,
        .function = function,
        .userData = userData
    };

    this->registeredFunctions.Append(info);
    return Result::Success;
}

}
