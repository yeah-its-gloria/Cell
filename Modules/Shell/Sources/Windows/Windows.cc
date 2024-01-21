// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Shell/Implementations/Windows.hh>

namespace Cell::Shell::Implementations {

Result Windows::RunDispatchImpl() {
    MSG message = { nullptr, 0, 0, 0, 0, { 0, 0 } };
    while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE) != FALSE) {
        DispatchMessageW(&message);
    }

    if (message.message == WM_QUIT) {
        this->isDone = true;
        return Result::RequestedQuit;
    }

    return Result::Success;
}

Result Windows::RequestQuit() {
    PostMessageW(this->window, WM_CLOSE, 0, 0);
    return Result::Success;
}

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
