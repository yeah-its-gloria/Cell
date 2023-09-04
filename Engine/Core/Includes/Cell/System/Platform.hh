// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/System/Event.hh>
#include <Cell/System/Mutex.hh>
#include <Cell/System/Thread.hh>

namespace Cell::Shell { class Input; }

namespace Cell::System {

// Abstract platform interface.
class IPlatform : public Object {
public:
    // Blocks execution until the platform signals it's fully ready.
    CELL_FUNCTION void BlockUntilReady() const;

    // Checks whether the title should still be active.
    CELL_NODISCARD CELL_FUNCTION bool IsStillActive() const;

    // Signals that the platform should finish execution.
    CELL_FUNCTION void SignalEnd();

    // Creates an input handler.
    CELL_FUNCTION Shell::Input* CreateInputHandler();

protected:
    Thread* mainThread = nullptr;
    Event* readySignal = nullptr;
    Event* exitSignal  = nullptr;
};

}
