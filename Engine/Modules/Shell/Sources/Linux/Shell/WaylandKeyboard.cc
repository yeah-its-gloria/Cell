// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/Linux.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Panic.hh>

#include <sys/mman.h>
#include <unistd.h>

namespace Cell::Shell::Implementations {

void Linux::WaylandKeyboardKeymap(void* data, struct wl_keyboard* keyboard, const uint32_t format, const int32_t fd, const uint32_t size) {
    (void)(keyboard);
    (void)(fd);
    (void)(size);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);

    CELL_ASSERT(format == WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1);

    void* mapped = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
    CELL_ASSERT(mapped != MAP_FAILED);

    struct xkb_keymap* keymap = xkb_keymap_new_from_string(_linux->keyboardContext, (char*)mapped, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
    CELL_ASSERT(keymap != nullptr);

    munmap(mapped, size);
    close(fd);

    struct xkb_state* state = xkb_state_new(keymap);

    xkb_keymap_unref(_linux->keyboardKeymap);
    xkb_state_unref(_linux->keyboardState);

    _linux->keyboardKeymap = keymap;
    _linux->keyboardState = state;
}

void Linux::WaylandKeyboardEnter(void* data, struct wl_keyboard* keyboard, uint32_t serial, struct wl_surface* surface, struct wl_array* keys) {
    (void)(keyboard);
    (void)(serial);
    (void)(surface);
    (void)(keys);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);

    // TODO: implement
}

void Linux::WaylandKeyboardLeave(void* data, struct wl_keyboard* keyboard, uint32_t serial, struct wl_surface* surface) {
    (void)(keyboard);
    (void)(serial);
    (void)(surface);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);

    // TODO: implement
}

void Linux::WaylandKeyboardKey(void* data, struct wl_keyboard* keyboard, const uint32_t serial, const uint32_t time, const uint32_t key, const uint32_t state) {
    (void)(keyboard);
    (void)(serial);
    (void)(time);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);

    _linux->keyLock->Lock();

    xkb_keysym_t sym = xkb_state_key_get_one_sym(_linux->keyboardState, key + 8);
    switch (sym) {
    case 65307: {// Escape
        _linux->exitSignal->Signal();
        break;
    }

    case 113: {// Q
        if (state & WL_KEYBOARD_KEY_STATE_PRESSED) {
            _linux->keys |= Shell::KeyValue::Q;
        } else {
            _linux->keys ^= Shell::KeyValue::Q;
        }

        break;
    }

    case 119: {// W
        if (state & WL_KEYBOARD_KEY_STATE_PRESSED) {
            _linux->keys |= Shell::KeyValue::W;
        } else {
            _linux->keys ^= Shell::KeyValue::W;
        }

        break;
    }

    case 101: {// E
        if (state & WL_KEYBOARD_KEY_STATE_PRESSED) {
            _linux->keys |= Shell::KeyValue::E;
        } else {
            _linux->keys ^= Shell::KeyValue::E;
        }

        break;
    }

    case 97: {// A
        if (state & WL_KEYBOARD_KEY_STATE_PRESSED) {
            _linux->keys |= Shell::KeyValue::A;
        } else {
            _linux->keys ^= Shell::KeyValue::A;
        }

        break;
    }

    case 115: {// S
        if (state & WL_KEYBOARD_KEY_STATE_PRESSED) {
            _linux->keys |= Shell::KeyValue::S;
        } else {
            _linux->keys ^= Shell::KeyValue::S;
        }

        break;
    }

    case 100: {// D
        if (state & WL_KEYBOARD_KEY_STATE_PRESSED) {
            _linux->keys |= Shell::KeyValue::D;
        } else {
            _linux->keys ^= Shell::KeyValue::D;
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

    _linux->keyLock->Unlock();
}

void Linux::WaylandKeyboardModifiers(void* data,
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

void Linux::WaylandKeyboardRepeatInfo(void* data, struct wl_keyboard* keyboard, const int32_t rate, const int32_t delay) {
    (void)(keyboard);
    (void)(rate);
    (void)(delay);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);

    // TODO: implement
}

}
