// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Manifest.rc.h"

#include <Cell/Scoped.hh>
#include <Cell/Shell/Implementations/Windows.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Shell::Implementations {

Wrapped<Windows*, Result> Windows::New(const System::String& title) {
    HINSTANCE instance = GetModuleHandleW(nullptr);

    const WNDCLASSEXW windowClass = {
        .cbSize = sizeof(WNDCLASSEXW),
        .style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC,
        .lpfnWndProc = Windows::WindowLoop,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = instance,
        .hIcon = LoadIconW(instance, MAKEINTRESOURCEW(ManifestIconID)),
        .hCursor = LoadCursorW(nullptr, IDC_ARROW),
        .hbrBackground = CreateSolidBrush(RGB(0, 0, 0)),
        .lpszMenuName = nullptr,
        .lpszClassName = L"Cell",
        .hIconSm = windowClass.hIcon
    };

    const ATOM atom = RegisterClassExW(&windowClass);
    if (atom == 0) {
        System::Panic("RegisterClassExW failed");
    }

    const DWORD exWindowStyle = WS_EX_WINDOWEDGE;
    const DWORD windowStyle = WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

    RECT defaultResolution = {
        .left = 60,
        .top = 60,
        .right = 60 + 1280,
        .bottom = 60 + 720
    };

    const BOOL win32Result = AdjustWindowRectExForDpi(&defaultResolution, windowStyle, FALSE, exWindowStyle, GetDpiForSystem());
    CELL_ASSERT(win32Result == TRUE);

    wchar_t* titleWide = title.IsEmpty() ? (wchar_t*)L"Cell" : title.ToPlatformWideString();
    HWND window = CreateWindowExW(exWindowStyle,
                                  windowClass.lpszClassName,
                                  titleWide,
                                  windowStyle | WS_OVERLAPPED,
                                  defaultResolution.left,
                                  defaultResolution.top,
                                  defaultResolution.right,
                                  defaultResolution.bottom,
                                  nullptr,
                                  nullptr,
                                  instance,
                                  nullptr
    );

    if (!title.IsEmpty()) {
        System::FreeMemory(titleWide);
    }

    if (window == nullptr) {
        System::Panic("CreateWindowExW failed");
    }

    Windows* windows = new Windows(instance, window, windowClass);

    SetLastError(ERROR_SUCCESS); // SetWindowLongPtr does not clear errors
    SetWindowLongPtrW(window, GWLP_USERDATA, (LONG_PTR)windows);
    if (GetLastError() != ERROR_SUCCESS) {
        System::Panic("SetWindowLongPtrW failed");
    }

    return windows;
}

Windows::~Windows() {
    BOOL win32Result = DestroyWindow(this->window);
    CELL_ASSERT(win32Result == TRUE);

    win32Result = DeleteObject(this->windowClass.hbrBackground);
    CELL_ASSERT(win32Result == TRUE);

    win32Result = UnregisterClassW(this->windowClass.lpszClassName, this->instance);
    CELL_ASSERT(win32Result == TRUE);
}

}
