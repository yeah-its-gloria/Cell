// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/Shell/InputTypes.hh>
#include <Cell/Shell/Result.hh>
#include <Cell/System/String.hh>

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

    // Returns whether the application is still in the foreground.
    CELL_NODISCARD CELL_INLINE bool IsInForeground() const {
        return this->isActivated;
    }

    // Returns the currently pressed keys.
    CELL_NODISCARD CELL_INLINE KeyboardButton GetKeys() const {
        return this->keys;
    }

    CELL_NODISCARD CELL_INLINE KeyboardButton GetPreviousKeys() const {
        return this->oldKeys;
    }

    // Acknowledges the current keys and moves them to old key storage.
    CELL_INLINE void MoveKeysToOld() {
        this->oldKeys = this->keys;
    }

    // Creates an input handler.
    CELL_FUNCTION Input* CreateInputHandler();

    // Runs all updates.
    virtual Result RunDispatch() = 0;

    // Gets the count of the drawable area of the current window.
    virtual Wrapped<Extent, Result> GetDrawableExtent() = 0;

    // Sets a new drawable area count for the current window.
    virtual Result SetDrawableExtent(const Extent extent) = 0;

    // Sets a new window title.
    virtual Result SetNewTitle(const System::String& title) = 0;

protected:
    bool isActivated = true;
    bool isDone = false;

    KeyboardButton keys = KeyboardButton::None;
    KeyboardButton oldKeys = KeyboardButton::None;
};

// Sets up the most suited shell implementation for the platform.
CELL_FUNCTION Wrapped<IShell*, Result> CreateShell(const System::String& title = "");

}
