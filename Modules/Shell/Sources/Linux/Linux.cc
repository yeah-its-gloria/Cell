// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Shell/Implementations/Linux.hh>

namespace Cell::Shell::Implementations {

Result Linux::RunDispatch() {
    const int waylandResult = wl_display_dispatch_pending(this->display);
    CELL_ASSERT(waylandResult > -1);

    if (this->xdgRequestedClose) {
        return Result::RequestedQuit;
    }

    return Result::Success;
}

Result Linux::RequestQuit() {
    this->xdgRequestedClose = true;
    return Result::Success;
}

Wrapped<Extent, Result> Linux::GetDrawableExtent() {
    // TODO: don't hardcode this lol

    return Extent { 1280, 720 };
}

Result Linux::SetDrawableExtent(const Extent extent) {
    (void)(extent);

    // TODO: implement

    return Result::Success;
}

Result Linux::SetNewTitle(const System::String& title) {
    if (title.IsEmpty()) {
        return Result::InvalidParameters;
    }

    ScopedBlock<char> titleCChar = title.ToCharPointer();
    xdg_toplevel_set_title(this->xdgToplevel, &titleCChar);

    return Result::Success;
}

}
