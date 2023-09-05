// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/Linux.hh>
#include <Cell/System/Panic.hh>

#include <string.h>

#define IS_INTERFACE(_name) (strcmp(interface, _name##_interface.name) == 0)

namespace Cell::Shell::Implementations {

void Linux::WaylandRegistryAdd(void* data, struct wl_registry* registry, const uint32_t name, const char* interface, const uint32_t version) {
    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr && registry != nullptr);

    if (IS_INTERFACE(wl_compositor)) {
        if (version < 5) {
            System::Panic("wl_compositor interface version %d is too old (required: 5)", version);
        }

        _linux->compositor = (struct wl_compositor*)wl_registry_bind(registry, name, &wl_compositor_interface, 5);
        if (_linux->compositor == nullptr) {
            System::Panic("wl_registry_bind for wl_compositor failed");
        }

        return;
    }

    if (IS_INTERFACE(wl_seat)) {
        if (version < 7) {
            System::Panic("xdg_wm_base interface version %d is too old (required: 7)", version);
        }

        _linux->seat = (struct wl_seat*)wl_registry_bind(registry, name, &wl_seat_interface, 2);
        if (_linux->seat == nullptr) {
            System::Panic("wl_registry_bind for wl_seat failed");
        }

        wl_seat_add_listener(_linux->seat, &Linux::seatListener, _linux);
        return;
    }

    if (IS_INTERFACE(xdg_wm_base)) {
        if (version < 2) {
            System::Panic("xdg_wm_base interface version %d is too old (required: 2)", version);
        }

        _linux->xdgManager = (struct xdg_wm_base*)wl_registry_bind(registry, name, &xdg_wm_base_interface, 2);
        if (_linux->xdgManager == nullptr) {
            System::Panic("wl_registry_bind for xdg_wm_base failed");
        }

        xdg_wm_base_add_listener(_linux->xdgManager, &Linux::xdgManagerListener, _linux);
        return;
    }

    if (IS_INTERFACE(zxdg_decoration_manager_v1)) {
        if (version < 1) {
            System::Panic("zxdg_decoration_manager_v1 interface version %d is too old (required: 1)", version);
        }

        _linux->xdgDecorationManager = (struct zxdg_decoration_manager_v1*)wl_registry_bind(registry, name, &zxdg_decoration_manager_v1_interface, 1);
        if (_linux->xdgDecorationManager == nullptr) {
            System::Panic("wl_registry_bind for zxdg_decoration_manager_v1 failed");
        }

        return;
    }

    if (IS_INTERFACE(zwp_idle_inhibit_manager_v1)) {
        if (version < 1) {
            System::Panic("zwp_idle_inhibit_manager_v1 version %d is too old (required: 1)", version);
        }

        _linux->idleInhibitManager = (struct zwp_idle_inhibit_manager_v1*)wl_registry_bind(registry, name, &zwp_idle_inhibit_manager_v1_interface, 1);
        if (_linux->idleInhibitManager == nullptr) {
            System::Panic("wl_registry_bind for zwp_idle_inhibit_manager_v1 failed");
        }

        return;
    }
}

void Linux::WaylandRegistryRemove(void* data, struct wl_registry* registry, const uint32_t name) {
    (void)(registry);
    (void)(name);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr);
}

}
