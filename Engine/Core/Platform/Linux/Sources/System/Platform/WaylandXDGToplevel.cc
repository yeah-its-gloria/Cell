// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Linux.hh>

namespace Cell::System::Platform {

void Linux::WaylandXDGToplevelConfigure(void* data, struct xdg_toplevel* xdg_toplevel, const int width, const int height, struct wl_array* states) {
    (void)(width);
    (void)(height);
    (void)(states);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr && xdg_toplevel != nullptr);
}

void Linux::WaylandXDGToplevelClose(void* data, struct xdg_toplevel* xdg_toplevel) {
    (void)(xdg_toplevel);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);

    _linux->xdgCloseSignal->Signal();
}

void Linux::WaylandXDGToplevelConfigureBounds(void* data, struct xdg_toplevel* xdg_toplevel, const int32_t width, const int32_t height) {
    (void)(width);
    (void)(height);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr && xdg_toplevel != nullptr);
}

void Linux::WaylandXDGToplevelAnnounceWMCapabilities(void* data, struct xdg_toplevel* xdg_toplevel, struct wl_array* capabilities) {
    (void)(capabilities);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr && xdg_toplevel != nullptr);
}

}
