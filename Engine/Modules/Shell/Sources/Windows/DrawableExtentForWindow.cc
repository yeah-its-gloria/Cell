// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/Windows.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Shell::Implementations {

Wrapped<Extent, Result> Windows::GetDrawableExtent() {
    RECT rect = {0, 0, 0, 0};
    const BOOL result = GetClientRect(this->window, &rect);
    if (result == FALSE) {
        switch (GetLastError()) {
        default: {
            System::Panic("GetClientRect failed");
        }
        }
    }

    return Extent {(uint32_t)rect.right - rect.left, (uint32_t)rect.bottom - rect.top};
}

Result Windows::SetDrawableExtent(const Extent extent) {
    const BOOL result = SetWindowPos(this->window, nullptr, 0, 0, extent.width, extent.height, SWP_NOMOVE);
    if (result == FALSE) {
        switch (GetLastError()) {
        default: {
            System::Panic("SetWindowPos failed");
        }
        }
    }

    return Result::Success;
}

}
