// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/Linux.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Panic.hh>

#include <sys/mman.h>
#include <unistd.h>

namespace Cell::Shell::Implementations {

void Linux::PointerEnter(void* data, struct wl_pointer* pointer, const uint32_t serial, struct wl_surface* surface, const wl_fixed_t x, const wl_fixed_t y) {
    (void)(surface); (void)(x); (void)(y);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr && pointer != nullptr);

    _linux->pointerActive = true;
    _linux->pointerSerial = serial;
}

void Linux::PointerLeave(void* data, struct wl_pointer* pointer, const uint32_t serial, struct wl_surface* surface) {
    (void)(surface);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr && pointer != nullptr);

    _linux->pointerActive = false;
    _linux->pointerSerial = serial;
}

void Linux::PointerMotion(void* data, struct wl_pointer* pointer, const uint32_t time, const wl_fixed_t x, const wl_fixed_t y) {
    (void)(time); (void)(x); (void)(y);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr && pointer != nullptr);
}

void Linux::PointerButton(void* data, struct wl_pointer* pointer, const uint32_t serial, const uint32_t time, const uint32_t button, const uint32_t state) {
    (void)(serial); (void)(time); (void)(button); (void)(state);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr && pointer != nullptr);
}

void Linux::PointerAxis(void* data, struct wl_pointer* pointer, const uint32_t time, const uint32_t axis, const wl_fixed_t value) {
    (void)(time); (void)(axis); (void)(value);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr && pointer != nullptr);
}

void Linux::PointerFrameEnd(void* data, struct wl_pointer* pointer) {
    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr && pointer != nullptr);
}

void Linux::PointerAxisStop(void* data, struct wl_pointer* pointer, const uint32_t time, const uint32_t axis) {
    (void)(time); (void)(axis);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr && pointer != nullptr);
}

}
