// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Manifest.rc.h"
#include <Cell/Scoped.hh>
#include <Cell/Shell/Implementations/Windows.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Shell::Implementations {

Windows::~Windows() {
    BOOL win32Result = DestroyWindow(this->window);
    CELL_ASSERT(win32Result);

    win32Result = DestroyIcon(this->_class.hIcon);
    CELL_ASSERT(win32Result);

    win32Result = DestroyCursor(this->_class.hCursor);
    CELL_ASSERT(win32Result);

    win32Result = DeleteObject(this->_class.hbrBackground);
    CELL_ASSERT(win32Result);

    win32Result = UnregisterClassW(this->_class.lpszClassName, this->instance);
    CELL_ASSERT(win32Result);
}

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
        .hIconSm = windowClass.hIcon};

    const ATOM atom = RegisterClassExW(&windowClass);
    if (atom == 0) {
        // TODO: error checking
        System::Panic("RegisterClassExW failed");
    }

    const DWORD exWindowStyle = WS_EX_WINDOWEDGE;
    const DWORD windowStyle = WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

    RECT defaultResolution = {
        .left = 60,
        .top = 60,
        .right = 60 + 1280,
        .bottom = 60 + 720};

    const BOOL win32Result = AdjustWindowRectEx(&defaultResolution, windowStyle, FALSE, exWindowStyle);
    if (win32Result == FALSE) {
        // TODO: error checking
        System::Panic("AdjustWindowRectEx failed");
    }

    wchar_t* titleWide = title.IsEmpty() ? (wchar_t*)L"Cell" : title.ToPlatformWideString();
    HWND window = CreateWindowExW(
        exWindowStyle,
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
        nullptr);

    if (!title.IsEmpty()) {
        System::FreeMemory(titleWide);
    }

    if (window == nullptr) {
        // TODO: error checking
        System::Panic("CreateWindowExW failed");
    }

    Windows* windows = new Windows(instance, window, windowClass);

    SetWindowLongPtrW(window, GWLP_USERDATA, (LONG_PTR)windows);
    switch (GetLastError()) {
    case ERROR_SUCCESS: {
        break;
    }

    // TODO: error checking

    default: {
        System::Panic("SetWindowLongPtrW failed");
    }
    }

    return windows;
}

Result Windows::RunDispatch() {
    bool hasProcessed = false;

    MSG message;
    while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE) == TRUE) {
        hasProcessed = true;

        TranslateMessage(&message);
        DispatchMessageW(&message);
    }

    if (!hasProcessed) {
        return Result::NoUpdates;
    }

    if (message.message == WM_QUIT) {
        this->isDone = true;
        return Result::RequestedQuit;
    }

    return Result::Success;
}

}
