// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Shell/Result.hh>
#include <Cell/System/Platform.hh>

namespace Cell::Shell {

// Extent for window functions.
struct Extent {
    uint32_t width;
    uint32_t height;
};

// Gets the count of the drawable area of the current window.
CELL_FUNCTION Wrapped<Extent, Result> GetDrawableExtentForWindow(System::IPlatform& platform);

// Sets a new drawable area count for the current window.
CELL_FUNCTION Result SetDrawableExtentForWindow(System::IPlatform& platform, const Extent extent);

// Sets a new window title.
CELL_FUNCTION Result SetNewTitleForWindow(System::IPlatform& platform, const System::String& title);

}
