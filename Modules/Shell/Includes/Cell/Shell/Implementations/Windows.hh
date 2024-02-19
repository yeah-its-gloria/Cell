// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Shell/Shell.hh>
#include <Cell/System/Mutex.hh>
#include <Cell/System/Platform/Windows/Includes.h>

namespace Cell::Shell::Implementations {

// Shell implementation for Windows.
class Windows : public IShell {
public:
    // Creates a new shell instance.
    CELL_FUNCTION static Wrapped<Windows*, Result> New(const String& title, const Extent extent);

    // Destructs the shell instance.
    CELL_FUNCTION ~Windows() override;

    // Returns a handle to the instance of the currently running executable.
    CELL_INLINE HINSTANCE GetInstance() { return this->instance; }

    // Returns a handle to the engine window.
    CELL_INLINE HWND GetWindow() { return this->window; }

    CELL_FUNCTION Result RequestQuit() override;
    CELL_FUNCTION Wrapped<Extent, Result> GetDrawableExtent() override;
    CELL_FUNCTION Result SetDrawableExtent(const Extent extent) override;
    CELL_FUNCTION Result SetNewTitle(const String& title) override;
    CELL_FUNCTION Result IndicateStatus(const ShellStatus status) override;
    CELL_FUNCTION Result CaptureState(const bool captured) override;

    CELL_NON_COPYABLE(Windows)

private:
    CELL_INLINE Windows(HINSTANCE CELL_NONNULL instance, HWND CELL_NONNULL window, WNDCLASSEXW windowClass) : instance(instance), window(window), windowClass(windowClass) { }

    CELL_FUNCTION_INTERNAL Result RunDispatchImpl() override;

    CELL_FUNCTION_INTERNAL static LRESULT WindowLoop(HWND window, UINT message, WPARAM paramHigh, LPARAM paramLow);
    CELL_FUNCTION_INTERNAL void HandleKeyInput(const WPARAM key, const LPARAM extInfo, const bool isPressed);

    HINSTANCE instance;
    HWND window;
    WNDCLASSEXW windowClass;

    bool shouldCapture = false;

    int prevCoordX = 0;
    int prevCoordY = 0;
};

}
