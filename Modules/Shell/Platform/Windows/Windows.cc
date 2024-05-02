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

Result Windows::Log(const String& text) {
    ScopedBlock<wchar_t> textWide = text.ToPlatformWideString();

    HWND textWnd = CreateWindowExW(0, L"STATIC", textWide, WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 30 * this->messages.GetCount(), 1000, 30, window, nullptr, instance, nullptr);
    if (textWnd == nullptr) {
        System::Panic("CreateWindowExW failed");
    }

    SendMessage(textWnd, WM_SETFONT, (WPARAM)this->font, (LPARAM)MAKELONG(TRUE, 0));

    this->messages.Append(textWnd);
    return Result::Success;
}

Result Windows::LogClear() {
    for (HWND& text : this->messages) {
        DestroyWindow(text);
    }

    this->messages.Reset();
    return Result::Success;
}

}
