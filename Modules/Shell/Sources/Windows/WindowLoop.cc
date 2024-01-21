// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/Windows.hh>
#include <Cell/System/Timer.hh>

#include <dwmapi.h>

namespace Cell::Shell::Implementations {

LRESULT Windows::WindowLoop(HWND window, UINT message, WPARAM paramHigh, LPARAM paramLow) {
    Windows* windows = (Windows*)GetWindowLongPtrW(window, GWLP_USERDATA);

    switch (message) {
    case WM_CREATE:
    case WM_SETTINGCHANGE: {
        const BOOL value = TRUE;
        const HRESULT result = DwmSetWindowAttribute(window, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(BOOL));
        CELL_ASSERT(result == S_OK);

        return 0;
    }

    case WM_ACTIVATE: {
        // Apparently this is called very early, so the pointer is null
        if (windows != nullptr) {
            windows->isActivated = paramHigh == WA_ACTIVE || paramHigh == WA_CLICKACTIVE;
            if (!windows->isActivated) {
                windows->keys = KeyboardButton::None;
            }
        }

        return 0;
    }

    case WM_CLOSE: {
        PostQuitMessage(0);
        return 0;
    }

    case WM_GETMINMAXINFO: {
        MINMAXINFO* resolution = (MINMAXINFO*)paramLow;

        resolution->ptMinTrackSize.x = 1280;
        resolution->ptMinTrackSize.y = 720;

        resolution->ptMaxTrackSize.x = 4096;
        resolution->ptMaxTrackSize.y = 2160;

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

    case WM_MOVE: {
        if (windows != nullptr) {
            // prevent window moves from gluing keys
            windows->keys = KeyboardButton::None;
        }

        // otherwise unhandled
        break;
    }

    default: {
        break;
    }
    }

    return DefWindowProc(window, message, paramHigh, paramLow);
}

}
