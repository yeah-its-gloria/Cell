// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/Linux.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Panic.hh>

#include <string.h>

#define IS_INTERFACE(n) (strcmp(interface, n##_interface.name) == 0)

#define REGISTER_GLOBAL(n, rv, o) \
if (strcmp(interface, n##_interface.name) == 0) { \
    if (version < rv) { System::Panic(#n" interface version %d is too old (required: "#rv")", version); } \
    _linux->o = (struct n*)wl_registry_bind(registry, identity, &n##_interface, rv);\
    if (_linux->o == nullptr) { System::Panic("wl_registry_bind for "#n" failed"); } \
}

#define REGISTER_GLOBAL_LISTENER(n, rv, o, l) \
if (strcmp(interface, n##_interface.name) == 0) { \
    if (version < rv) { System::Panic(#n" interface version %d is too old (required: "#rv")", version); } \
    _linux->o = (struct n*)wl_registry_bind(registry, identity, &n##_interface, rv);\
    if (_linux->o == nullptr) { System::Panic("wl_registry_bind for "#n" failed"); } \
    n##_add_listener(_linux->o, &Linux::l, _linux); \
}

namespace Cell::Shell::Implementations {

void Linux::RegistryAdd(void* data, struct wl_registry* registry, const uint32_t identity, const char* interface, const uint32_t version) {
    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr && registry != nullptr);

    REGISTER_GLOBAL(wl_compositor, 4, compositor)
    REGISTER_GLOBAL_LISTENER(wl_seat, 7, seat, SeatListener)
    REGISTER_GLOBAL_LISTENER(xdg_wm_base, 1, xdgManager, XDGManagerListener)
    REGISTER_GLOBAL(zxdg_decoration_manager_v1, 1, xdgDecorationManager)
    REGISTER_GLOBAL(zwp_idle_inhibit_manager_v1, 1, idleInhibitManager)
    REGISTER_GLOBAL(wp_cursor_shape_manager_v1, 1, cursorShapeManager)
    REGISTER_GLOBAL(zwp_pointer_constraints_v1, 1, pointerConstraints)
    REGISTER_GLOBAL(zwp_relative_pointer_manager_v1, 1, relativePointerManager)
}

void Linux::RegistryRemove(void* data, struct wl_registry* registry, const uint32_t identity) {
    (void)(identity);

    Linux* _linux = (Linux*)data;
    CELL_ASSERT(_linux != nullptr && registry != nullptr);
}

}
