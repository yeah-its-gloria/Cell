// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Shell/KeyValue.hh>
#include <Cell/Shell/Shell.hh>
#include <Cell/System/Mutex.hh>
#include <Cell/System/String.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::Shell::Implementations {

// Shell implementation for Windows.
class Windows : public IShell {
friend Input;

public:
    // Creates a new shell instance.
    CELL_FUNCTION static Wrapped<Windows*, Result> New(const System::String& title = "");

    // Destructs the shell instance.
    CELL_FUNCTION ~Windows() override;

    // Returns a handle to the instance of the currently running executable.
    CELL_INLINE HINSTANCE GetInstance() { return this->instance; }

    // Returns a handle to the engine window.
    CELL_INLINE HWND GetWindow() { return this->window; }

    Result RunDispatch() override;
    Wrapped<Extent, Result> GetDrawableExtent() override;
    Result SetDrawableExtent(const Extent extent) override;
    Result SetNewTitle(const System::String& title) override;

private:
    CELL_FUNCTION_INTERNAL Windows(HINSTANCE CELL_NONNULL instance, HWND CELL_NONNULL window, WNDCLASSEXW _class) : instance(instance), window(window), _class(_class) { }

    CELL_FUNCTION_INTERNAL static LRESULT WindowLoop(HWND window, UINT message, WPARAM paramHigh, LPARAM paramLow);
    CELL_FUNCTION_INTERNAL void HandleKeyInput(WPARAM key, bool isPressed);

    HINSTANCE instance;
    HWND window;
    WNDCLASSEXW _class;

    System::Mutex keyLock;
    KeyValue keys = KeyValue::None;
};

}
