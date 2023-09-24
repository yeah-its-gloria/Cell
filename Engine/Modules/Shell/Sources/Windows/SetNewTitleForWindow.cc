// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Shell/Implementations/Windows.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Shell::Implementations {

Result Windows::SetNewTitle(const System::String& title) {
    if (title.IsEmpty()) {
        return Result::InvalidParameters;
    }

    ScopedBlock<wchar_t> titleWide = title.ToPlatformWideString();
    const BOOL result = SetWindowTextW(this->window, &titleWide);
    if (result == FALSE) {
        switch (GetLastError()) {
        default: {
            System::Panic("SetWindowTextW failed");
        }
        }
    }

    return Result::Success;
}

}
