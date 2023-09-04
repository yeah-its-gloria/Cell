// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Input.hh>
#include <Cell/System/Platform.hh>

namespace Cell::System {

void IPlatform::BlockUntilReady() const {
    this->readySignal->Wait();
}

bool IPlatform::IsStillActive() const {
    return this->exitSignal->Wait(1) == EventWaitState::Timeout;
}

void IPlatform::SignalEnd() {
    return this->exitSignal->Signal();
}

Shell::Input* IPlatform::CreateInputHandler() {
    return new Shell::Input(*this);
}

}
