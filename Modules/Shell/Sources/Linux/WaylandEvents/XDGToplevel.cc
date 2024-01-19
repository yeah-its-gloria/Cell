// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/Linux.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Shell::Implementations {

void Linux::WaylandXDGToplevelConfigure(void* data, struct xdg_toplevel* toplevel, const int width, const int height, struct wl_array* states) {
    (void)(width);
    (void)(height);
    (void)(states);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr && toplevel != nullptr);
}

void Linux::WaylandXDGToplevelClose(void* data, struct xdg_toplevel* toplevel) {
    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr && toplevel != nullptr);

    _linux->isDone = true;
}

void Linux::WaylandXDGToplevelConfigureBounds(void* data, struct xdg_toplevel* toplevel, const int32_t width, const int32_t height) {
    (void)(width);
    (void)(height);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr && toplevel != nullptr);
}

void Linux::WaylandXDGToplevelAnnounceWMCapabilities(void* data, struct xdg_toplevel* toplevel, struct wl_array* capabilities) {
    (void)(capabilities);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr && toplevel != nullptr);
}

}
