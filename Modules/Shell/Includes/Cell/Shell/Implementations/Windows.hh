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
    CELL_FUNCTION static Wrapped<Windows*, Result> New(const String& title, const Extent extent);
    CELL_FUNCTION ~Windows();

    CELL_NODISCARD CELL_FUNCTION_TEMPLATE HINSTANCE GetInstance() const { return this->instance; }
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE HWND GetWindow() const { return this->window; }

    CELL_FUNCTION Result RequestQuit() override;
    CELL_FUNCTION Wrapped<Extent, Result> GetDrawableExtent() override;
    CELL_FUNCTION Result SetDrawableExtent(const Extent extent) override;
    CELL_FUNCTION Result SetNewTitle(const String& title) override;
    CELL_FUNCTION Result IndicateStatus(const ShellStatus status) override;
    CELL_FUNCTION Result CaptureState(const bool captured) override;
    CELL_FUNCTION Result Log(const String& text) override;
    CELL_FUNCTION Result LogClear() override;

private:
    CELL_FUNCTION_INTERNAL Windows(HINSTANCE CELL_NONNULL i, HWND CELL_NONNULL w, HFONT CELL_NONNULL f, const WNDCLASSEXW& c) : instance(i), window(w), font(f), windowClass(c) { }

    CELL_FUNCTION_INTERNAL Result RunDispatchImpl() override;

    CELL_FUNCTION_INTERNAL static LRESULT WindowLoop(HWND window, UINT message, WPARAM paramHigh, LPARAM paramLow);
    CELL_FUNCTION_INTERNAL void HandleKeyInput(const WPARAM key, const LPARAM extInfo, const bool isPressed);

    HINSTANCE instance;
    HWND window;
    HFONT font;
    WNDCLASSEXW windowClass;

    Collection::List<HWND> messages;

    bool shouldCapture = false;

    int prevCoordX = 0;
    int prevCoordY = 0;
};

}
