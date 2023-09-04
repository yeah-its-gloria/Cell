// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Window.hh>
#include <Cell/System/Platform/Linux.hh>

namespace Cell::Shell {

Wrapped<Extent, Result> GetDrawableExtentForWindow(System::IPlatform& platform) {
    System::Platform::Linux* _linux = (System::Platform::Linux*)&platform;
    if (_linux->GetWaylandSurface() == nullptr) {
        return Result::InvalidPlatform;
    }

    // TODO: don't hardcode this lol

    return Extent { 1280, 720 };
}

Result SetDrawableExtentForWindow(System::IPlatform& platform, const Extent extent) {
    System::Platform::Linux* _linux = (System::Platform::Linux*)&platform;
    if (_linux->GetWaylandSurface() == nullptr) {
        return Result::InvalidPlatform;
    }

    (void)(extent);

    // TODO: implement

    return Result::Success;
}


}
