// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/Linux.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Shell::Implementations {

void Linux::WaylandSeatCapabilities(void* data, struct wl_seat* seat, const uint32_t capabilities) {
    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);

    if ((capabilities & WL_SEAT_CAPABILITY_KEYBOARD) && _linux->keyboard == nullptr) {
        _linux->keyboard = wl_seat_get_keyboard(seat);
        CELL_ASSERT(_linux->keyboard != nullptr);

        wl_keyboard_add_listener(_linux->keyboard, &Linux::KeyboardListener, _linux);
    }

    if ((capabilities & WL_SEAT_CAPABILITY_POINTER) && _linux->pointer == nullptr) {
        _linux->pointer = wl_seat_get_pointer(seat);
        CELL_ASSERT(_linux->pointer != nullptr);
    }
}

void Linux::WaylandSeatName(void* data, struct wl_seat* seat, const char* name) {
    (void)(seat);
    (void)(name);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);
}

}
