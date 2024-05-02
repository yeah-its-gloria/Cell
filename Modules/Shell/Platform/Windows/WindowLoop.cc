// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/Windows.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Timer.hh>
#include <Cell/Utilities/MinMaxClamp.hh>

#include <dwmapi.h>
#include <windowsx.h>

namespace Cell::Shell::Implementations {

LRESULT Windows::WindowLoop(HWND window, UINT message, WPARAM paramHigh, LPARAM paramLow) {
    Windows* windows = (Windows*)GetWindowLongPtrW(window, GWLP_USERDATA);
    if (windows != nullptr) {
        CELL_ASSERT(window == windows->window);
    }

    switch (message) {
    case WM_CREATE:
    case WM_SETTINGCHANGE: {
        const BOOL value = TRUE;
        const HRESULT result = DwmSetWindowAttribute(window, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(BOOL));
        CELL_ASSERT(result == S_OK);
        break;
    }

    case WM_ACTIVATE: {
        // Apparently this is called very early, so the pointer is null
        if (windows == nullptr) {
            break;
        }

        windows->isActivated = paramHigh == WA_ACTIVE || paramHigh == WA_CLICKACTIVE;
        if (!windows->isActivated) {
            windows->keys = KeyboardButton::None;

            if (windows->shouldCapture) {
                ClipCursor(nullptr);
                ReleaseCapture();
                SetCursor(LoadCursorW(nullptr, IDC_ARROW));
            }
        } else if (windows->isActivated && windows->shouldCapture) {
            RECT clientRect;
            GetClientRect(window, &clientRect);

            POINT windowPos = { 0, 0 };
            POINT size      = { clientRect.right, clientRect.bottom };
            POINT center    = { size.x / 2, size.y / 2 };

            ClientToScreen(window, &windowPos);
            ClientToScreen(window, &size);
            ClientToScreen(window, &center);

            RECT clipRect = { windowPos.x, windowPos.y, size.x, size.y };
            ClipCursor(&clipRect);

            SetCursorPos(center.x, center.y);
            SetCapture(window);
            SetCursor(nullptr);
        }

        return 0;
    }

    case WM_CLOSE: {
        PostQuitMessage(0);
        return 0;
    }

    case WM_GETMINMAXINFO: {
        MINMAXINFO* resolution = (MINMAXINFO*)paramLow;

        resolution->ptMinTrackSize.x = 100;
        resolution->ptMinTrackSize.y = 100;

        resolution->ptMaxTrackSize.x = 100000;
        resolution->ptMaxTrackSize.y = 100000;

        return 0;
    }

    case WM_SYSCOMMAND: {
        switch (GET_SC_WPARAM(paramHigh)) {
        case SC_MONITORPOWER: {
            // screen is powering on
            if (paramLow == -1) {
                break;
            }

            // intentional fallthrough
        }

        case SC_SCREENSAVE: {
            return 0;
        }

        default: {
            break;
        }
        }

        break;
    }

    case WM_KEYDOWN: {
        CELL_ASSERT(windows != nullptr);

        switch (paramHigh) {
        case VK_LWIN:
        case VK_RWIN: {
            // TODO: don't record input
            break;
        }

        default: {
            windows->HandleKeyInput(paramHigh, paramLow, true);
            return 0;
        }
        }

        break;
    }

    case WM_KEYUP: {
        CELL_ASSERT(windows != nullptr);

        switch (paramHigh) {
        case VK_LWIN:
        case VK_RWIN: {
            break;
        }

        default: {
            windows->HandleKeyInput(paramHigh, paramLow, false);
            return 0;
        }
        }

        break;
    }

    case WM_MOUSEMOVE: {
        CELL_ASSERT(windows != nullptr);
        if (!windows->isActivated || !windows->shouldCapture) {
            break;
        }

        // TODO: calculate mouse movement even if not captured

        RECT clientRect = { 0, 0, 0, 0 };
        GetClientRect(window, &clientRect);

        POINT center = { clientRect.right / 2, clientRect.bottom / 2 };

        ClientToScreen(window, (POINT*)&clientRect.left);
        ClientToScreen(window, (POINT*)&clientRect.right);

        ClientToScreen(window, &center);

        POINT coords = { GET_X_LPARAM(paramLow), GET_Y_LPARAM(paramLow) };

        if (coords.x != center.x) {
            windows->mouseX = (coords.x - center.x) + clientRect.left;
        }

        if (coords.y != center.y) {
            windows->mouseY = (coords.y - center.y) + clientRect.top;
        }

        SetCursorPos(center.x, center.y);
        return 0;
    }

    case WM_MOVE: {
        if (windows != nullptr) {
            // prevent window moves from gluing keys
            windows->keys = KeyboardButton::None;
        }

        // otherwise unhandled
        break;
    }

    case WM_CTLCOLORSTATIC: {
        CELL_ASSERT(windows != nullptr);

        HDC hdcStatic = (HDC)paramHigh;

        SetTextColor(hdcStatic, RGB(255, 255, 255));
        SetBkColor(hdcStatic, RGB(0, 0, 0));
        return (INT_PTR)windows->windowClass.hbrBackground;
    }

    default: {
        break;
    }
    }

    return DefWindowProc(window, message, paramHigh, paramLow);
}

}
