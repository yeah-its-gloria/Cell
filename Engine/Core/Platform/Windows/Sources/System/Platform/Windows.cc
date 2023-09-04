// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Manifest.rc.h"

#include <Cell/System/Panic.hh>
#include <Cell/System/Sleep.hh>
#include "Cell/System/String.hh"
#include <Cell/System/Platform/Windows.hh>

#include <roapi.h>
#include <WinSock2.h>

namespace Cell::System::Platform {

struct trampolineData {
    const EntryFunction entry;
    Reference<IPlatform> platform;
    Reference<String> parameterString;
};

CELL_FUNCTION_INTERNAL void cellEntryTrampoline(void* data) {
    const trampolineData* entryData = (const trampolineData*)data;
    CELL_ASSERT(entryData != nullptr);

    entryData->entry(entryData->platform, entryData->parameterString);
}

Windows::Windows(HINSTANCE instance, const bool needsWindow) : instance(instance), needsWindow(needsWindow) {
    this->readySignal = new Event();
    this->exitSignal = new Event();

    this->keyLock = new Mutex();
}

Windows::~Windows() {
    delete this->mainThread;

    delete this->keyLock;
    delete this->readySignal;
    delete this->exitSignal;
}

void Windows::Start(HINSTANCE instance, const EntryFunction entry, const bool needsWindow) {
    Windows windows(instance, needsWindow);

    String parameters = String::FromPlatformWideString(GetCommandLineW()).Unwrap();

    const trampolineData trampolineData = {
        .entry           = entry,
        .platform        = Reference<IPlatform>(windows),
        .parameterString = Reference(parameters)
    };

    windows.mainThread = new Thread(cellEntryTrampoline, (void*)&trampolineData, "Title Thread");

    const WNDCLASSEXW windowClass = {
        .cbSize        = sizeof(WNDCLASSEXW),
        .style         = CS_VREDRAW | CS_HREDRAW | CS_OWNDC,
        .lpfnWndProc   = Windows::WindowLoop,
        .cbClsExtra    = 0,
        .cbWndExtra    = 0,
        .hInstance     = windows.instance,
        .hIcon         = LoadIconW(windows.instance, MAKEINTRESOURCEW(ManifestIconID)),
        .hCursor       = LoadCursorW(nullptr, IDC_ARROW),
        .hbrBackground = CreateSolidBrush(RGB(0, 0, 0)),
        .lpszMenuName  = nullptr,
        .lpszClassName = L"Cell",
        .hIconSm       = windowClass.hIcon
    };

    BOOL win32Result = FALSE;
    if (needsWindow) {
        // Create window

        const ATOM atom = RegisterClassExW(&windowClass);
        CELL_ASSERT(atom > 0);

        const DWORD exWindowStyle = WS_EX_WINDOWEDGE;
        const DWORD windowStyle = WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

        RECT defaultResolution =
        {
            .left   = 60,
            .top    = 60,
            .right  = 60 + 1280,
            .bottom = 60 + 720
        };

        win32Result = AdjustWindowRectEx(&defaultResolution, windowStyle, FALSE, exWindowStyle);
        CELL_ASSERT(win32Result);

        windows.window = CreateWindowExW(
            exWindowStyle,
            windowClass.lpszClassName,
            L"Cell",
            windowStyle | WS_OVERLAPPED,
            defaultResolution.left,
            defaultResolution.top,
            defaultResolution.right,
            defaultResolution.bottom,
            nullptr,
            nullptr,
            windows.instance,
            nullptr
        );

        CELL_ASSERT(windows.window != nullptr);

        SetWindowLongPtrW(windows.window, GWLP_USERDATA, (LONG_PTR)&windows);
        const DWORD error_result = GetLastError();
        CELL_ASSERT(error_result == ERROR_SUCCESS);
    }

    // Set up networking

    WSADATA wsaData;
    int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    CELL_ASSERT(wsaResult == 0);

    // Set up COM

    const HRESULT comResult = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    CELL_ASSERT(SUCCEEDED(comResult));

    // Signal that the platform is ready

    windows.readySignal->Signal();

    // Window loop

    MSG message;
    while (windows.mainThread->IsActive()) {
        if (needsWindow) {
            while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&message);
                DispatchMessageW(&message);
            }

            if (message.message == WM_QUIT) {
                break;
            }
        }

        System::Sleep(1); // Sleeping is more efficient than just yielding the thread
    }

    // Wait for the main thread to act on the exit signal

    windows.exitSignal->Signal();
    windows.mainThread->Join();

    // Cleanup

    CoUninitialize();

    wsaResult = WSACleanup();
    CELL_ASSERT(wsaResult == 0);

    if (needsWindow) {
        win32Result = DestroyWindow(windows.window);
        CELL_ASSERT(win32Result);
    }

    win32Result = DestroyIcon(windowClass.hIcon);
    CELL_ASSERT(win32Result);

    win32Result = DestroyCursor(windowClass.hCursor);
    CELL_ASSERT(win32Result);

    win32Result = DeleteObject(windowClass.hbrBackground);
    CELL_ASSERT(win32Result);

    if (needsWindow) {
        win32Result = UnregisterClassW(windowClass.lpszClassName, windows.instance);
        CELL_ASSERT(win32Result);
    }
}

}
