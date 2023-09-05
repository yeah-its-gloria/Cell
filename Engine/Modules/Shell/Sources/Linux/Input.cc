// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/Linux.hh>
#include <Cell/Shell/Input.hh>

namespace Cell::Shell {
using namespace Implementations;

Result Input::Poll() {
    Linux* _linux = (Linux*)&this->shell;
    for (registerInfo info : this->registeredFunctions) {
        if ((_linux->keys & info.match) == info.match) {
            info.function(info.match, info.userData);
        }
    }

    return Result::Success;
}

}
