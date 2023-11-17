// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Panic.hh>
#include <Cell/Shell/Implementations/Linux.hh>

namespace Cell::Shell::Implementations {

void Linux::WaylandXDGManagerPing(void* data, struct xdg_wm_base* manager, const uint32_t serial) {
    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);

    xdg_wm_base_pong(manager, serial);
}

void Linux::WaylandXDGSurfaceConfigure(void* data, struct xdg_surface* surface, const uint32_t serial) {
    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);

    xdg_surface_ack_configure(surface, serial);
}

void Linux::WaylandXDGDecorationConfigure(void* data, struct zxdg_toplevel_decoration_v1* decoration, const uint32_t mode) {
    (void)(decoration);
    (void)(mode);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);
}

}
