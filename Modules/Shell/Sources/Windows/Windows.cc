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

Result Windows::SetNewTitle(const String& title) {
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

Result Windows::IndicateStatus(const ShellStatus status) {
    HCURSOR cursor = nullptr;

    switch (status) {
    case ShellStatus::Default: {
        cursor = LoadCursorW(nullptr, IDC_ARROW);
        break;
    }

    case ShellStatus::Working: {
        cursor = LoadCursorW(nullptr, IDC_WAIT);
        break;
    }

    default: {
        CELL_UNREACHABLE;
    }
    }

    CELL_ASSERT(cursor != nullptr);
    SetCursor(cursor);
    return Result::Success;
}


Result Windows::CaptureState(const bool captured) {
    if (!this->isActivated) {
        return Result::NotActive;
    }

    if (this->shouldCapture == captured) {
        return Result::Success;
    }

    this->shouldCapture = captured;
    if (captured) {
        SetCursor(nullptr);
        PostMessageW(this->window, WM_ACTIVATE, WA_ACTIVE, 0);
        return Result::Success;
    }

    ReleaseCapture();
    return this->IndicateStatus(ShellStatus::Default);
}

}
