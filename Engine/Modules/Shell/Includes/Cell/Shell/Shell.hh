// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Shell/Result.hh>
#include <Cell/System/Event.hh>
#include <Cell/System/Mutex.hh>
#include <Cell/System/Thread.hh>

namespace Cell::Shell {

struct Extent {
    uint32_t width;
    uint32_t height;
};

class Input;

// Base shell interface.
class IShell : public Object {
public:
    // Common destructor.
    virtual ~IShell() = default;

    // Checks whether the title should still be active.
    CELL_NODISCARD CELL_INLINE bool IsStillActive() const {
        return !this->isDone;
    }

    // Creates an input handler.
    CELL_FUNCTION Input* CreateInputHandler();

    // Runs all updates.
    virtual Result RunDispatch() = 0;

    // Gets the count of the drawable area of the current window.
    virtual Wrapped<Extent, Result> GetDrawableExtentForWindow() = 0;

    // Sets a new drawable area count for the current window.
    virtual Result SetDrawableExtentForWindow(const Extent extent) = 0;

    // Sets a new window title.
    virtual Result SetNewTitleForWindow(const System::String& title) = 0;

protected:
    bool isDone = false;
};

// Sets up the most suited shell implementation for the platform.
CELL_FUNCTION Wrapped<IShell*, Result> CreateShell(const System::String& title = "");

}
