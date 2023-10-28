// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/Windows.hh>
#include <Cell/Shell/Input.hh>

namespace Cell::Shell {
using namespace Implementations;

Result Input::Poll() {
    Windows* windows = (Windows*)&this->shell;
    if (!windows->isActivated) {
        return Result::Success;
    }

    windows->keyLock.Lock();

    for (RegisterInfo info : this->registeredFunctions) {
        if ((windows->keys & info.match) == info.match) {
            info.function(info.match, info.userData);
        }
    }

    windows->keyLock.Unlock();

    return Result::Success;
}

}
