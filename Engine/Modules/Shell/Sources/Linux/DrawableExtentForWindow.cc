// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/Linux.hh>

namespace Cell::Shell::Implementations {

Wrapped<Extent, Result> Linux::GetDrawableExtentForWindow() {
    // TODO: don't hardcode this lol

    return Extent { 1280, 720 };
}

Result Linux::SetDrawableExtentForWindow(const Extent extent) {
    (void)(extent);

    // TODO: implement

    return Result::Success;
}


}
