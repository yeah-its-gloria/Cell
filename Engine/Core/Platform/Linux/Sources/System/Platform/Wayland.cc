// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Linux.hh>

namespace Cell::System::Platform {

void Linux::WaylandXDGManagerPing(void* data, struct xdg_wm_base* xdg_manager, const uint32_t serial) {
    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);

    xdg_wm_base_pong(xdg_manager, serial);
}

void Linux::WaylandXDGSurfaceConfigure(void* data, struct xdg_surface* xdg_surface, const uint32_t serial) {
    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);

    xdg_surface_ack_configure(xdg_surface, serial);
}

void Linux::WaylandXDGDecorationConfigure(void* data, struct zxdg_toplevel_decoration_v1* xdg_decoration, const uint32_t mode) {
    (void)(xdg_decoration);
    (void)(mode);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);
}

}
