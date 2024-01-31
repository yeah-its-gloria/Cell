// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/Linux.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Panic.hh>

#include <sys/mman.h>
#include <unistd.h>

namespace Cell::Shell::Implementations {

void Linux::KeyboardKeymap(void* data, struct wl_keyboard* keyboard, const uint32_t format, const int32_t fd, const uint32_t size) {
    (void)(keyboard);
    (void)(fd);
    (void)(size);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);

    CELL_ASSERT(format == WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1);

    void* mapped = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    CELL_ASSERT(mapped != MAP_FAILED);

    _linux->keyboardKeymap = xkb_keymap_new_from_string(_linux->keyboardContext, (char*)mapped, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
    CELL_ASSERT(_linux->keyboardKeymap != nullptr);

    munmap(mapped, size);
    close(fd);

    _linux->keyboardState = xkb_state_new(_linux->keyboardKeymap);
    CELL_ASSERT(_linux->keyboardState != nullptr);
}

void Linux::KeyboardEnter(void* data, struct wl_keyboard* keyboard, uint32_t serial, struct wl_surface* surface, struct wl_array* keys) {
    (void)(keyboard);
    (void)(serial);
    (void)(surface);
    (void)(keys);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);

    _linux->keyboardActive = true;
}

void Linux::KeyboardLeave(void* data, struct wl_keyboard* keyboard, uint32_t serial, struct wl_surface* surface) {
    (void)(keyboard);
    (void)(serial);
    (void)(surface);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);

    _linux->keyboardActive = false;
}

void Linux::KeyboardKey(void* data, struct wl_keyboard* keyboard, const uint32_t serial, const uint32_t time, const uint32_t key, const uint32_t state) {
    (void)(keyboard);
    (void)(serial);
    (void)(time);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);

    // TODO: build a LUT

    xkb_keysym_t sym = xkb_state_key_get_one_sym(_linux->keyboardState, key + 8);
    switch (xkb_keysym_to_upper(sym)) {
    case XKB_KEY_Escape: {
        _linux->isDone = true;
        break;
    }

    case XKB_KEY_Tab: {
        if (state & WL_KEYBOARD_KEY_STATE_PRESSED) {
            _linux->keys |= Shell::KeyboardButton::Tab;
        } else {
            _linux->keys ^= Shell::KeyboardButton::Tab;
        }

        break;
    }

    case XKB_KEY_Q: {
        if (state & WL_KEYBOARD_KEY_STATE_PRESSED) {
            _linux->keys |= Shell::KeyboardButton::Q;
        } else {
            _linux->keys ^= Shell::KeyboardButton::Q;
        }

        break;
    }

    case XKB_KEY_W: {
        if (state & WL_KEYBOARD_KEY_STATE_PRESSED) {
            _linux->keys |= Shell::KeyboardButton::W;
        } else {
            _linux->keys ^= Shell::KeyboardButton::W;
        }

        break;
    }

    case XKB_KEY_R: {
        if (state & WL_KEYBOARD_KEY_STATE_PRESSED) {
            _linux->keys |= Shell::KeyboardButton::R;
        } else {
            _linux->keys ^= Shell::KeyboardButton::R;
        }

        break;
    }

    case XKB_KEY_E: {
        if (state & WL_KEYBOARD_KEY_STATE_PRESSED) {
            _linux->keys |= Shell::KeyboardButton::E;
        } else {
            _linux->keys ^= Shell::KeyboardButton::E;
        }

        break;
    }

    case XKB_KEY_A: {
        if (state & WL_KEYBOARD_KEY_STATE_PRESSED) {
            _linux->keys |= Shell::KeyboardButton::A;
        } else {
            _linux->keys ^= Shell::KeyboardButton::A;
        }

        break;
    }

    case XKB_KEY_S: {
        if (state & WL_KEYBOARD_KEY_STATE_PRESSED) {
            _linux->keys |= Shell::KeyboardButton::S;
        } else {
            _linux->keys ^= Shell::KeyboardButton::S;
        }

        break;
    }

    case XKB_KEY_D: {
        if (state & WL_KEYBOARD_KEY_STATE_PRESSED) {
            _linux->keys |= Shell::KeyboardButton::D;
        } else {
            _linux->keys ^= Shell::KeyboardButton::D;
        }

        break;
    }

    case XKB_KEY_F: {
        if (state & WL_KEYBOARD_KEY_STATE_PRESSED) {
            _linux->keys |= Shell::KeyboardButton::F;
        } else {
            _linux->keys ^= Shell::KeyboardButton::F;
        }

        break;
    }

    default: {
#ifdef _DEBUG
        char buf[129] = {0};
        xkb_state_key_get_utf8(_linux->keyboardState, key + 8, buf, sizeof(buf));
        System::Log("Unknown key: %d %s", sym, state & WL_KEYBOARD_KEY_STATE_PRESSED ? "pressed" : "released");
#endif

        break;
    }
    }
}

void Linux::KeyboardModifiers(void* data,
                                     struct wl_keyboard* keyboard,
                                     const uint32_t serial,
                                     const uint32_t modsDepressed,
                                     const uint32_t modsLatched,
                                     const uint32_t modsLocked,
                                     const uint32_t group) {
    (void)(keyboard);
    (void)(serial);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);

    xkb_state_update_mask(_linux->keyboardState, modsDepressed, modsLatched, modsLocked, 0, 0, group);
}

void Linux::KeyboardRepeatInfo(void* data, struct wl_keyboard* keyboard, const int32_t rate, const int32_t delay) {
    (void)(keyboard);
    (void)(rate);
    (void)(delay);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);

    // TODO: implement
}

}
