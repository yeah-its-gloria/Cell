// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Shell/KeyValue.hh>
#include <Cell/System/Entry.hh>
#include <Cell/System/Platform.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::System::Platform {

// Platform implementation for Windows/Win32.
class Windows : public IPlatform {
friend Shell::Input;

public:
    // Called by the bootstrapping code.
    CELL_FUNCTION static void Start(HINSTANCE instance, const EntryFunction entry, const bool needsWindow);

    // Returns a handle to the instance of the currently running executable.
    CELL_INLINE HINSTANCE GetInstance() { return this->instance; }

    // Returns a handle to the engine window.
    CELL_INLINE HWND GetWindow() {
        CELL_ASSERT(this->needsWindow);
        return this->window;
    }

private:
    CELL_FUNCTION_INTERNAL Windows(HINSTANCE instance, const bool needsWindow);
    CELL_FUNCTION_INTERNAL ~Windows();

    CELL_FUNCTION_INTERNAL static LRESULT WindowLoop(HWND window, UINT message, WPARAM paramHigh, LPARAM paramLow);
    CELL_FUNCTION_INTERNAL void HandleKeyInput(WPARAM key, bool isPressed);

    HINSTANCE instance = nullptr;
    HWND window = nullptr;
    bool needsWindow = true;

    Mutex* keyLock = nullptr;
    Shell::KeyValue keys = Shell::KeyValue::None;
};

}
