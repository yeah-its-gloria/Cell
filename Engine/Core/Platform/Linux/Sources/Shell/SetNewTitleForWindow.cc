// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Shell/Window.hh>
#include <Cell/System/Platform/Linux.hh>

namespace Cell::Shell {

Result SetNewTitleForWindow(System::IPlatform& platform, const System::String& title) {
    System::Platform::Linux* _linux = (System::Platform::Linux*)&platform;
    if (_linux->GetWaylandXDGToplevel() == nullptr) {
        return Result::InvalidPlatform;
    }

    if (title.IsEmpty()) {
        return Result::InvalidParameters;
    }

    ScopedBlock<char> titleCChar = title.ToCharPointer();
    xdg_toplevel_set_title(_linux->GetWaylandXDGToplevel(), &titleCChar);

    return Result::Success;
}

}
