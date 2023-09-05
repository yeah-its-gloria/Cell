// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Shell/Implementations/Linux.hh>

namespace Cell::Shell::Implementations {

Result Linux::SetNewTitleForWindow(const System::String& title) {
    if (title.IsEmpty()) {
        return Result::InvalidParameters;
    }

    ScopedBlock<char> titleCChar = title.ToCharPointer();
    xdg_toplevel_set_title(this->xdgToplevel, &titleCChar);

    return Result::Success;
}

}
