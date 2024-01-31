// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/Linux.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Memory.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Timer.hh>

namespace Cell::Shell::Implementations {
using namespace System;

Wrapped<Linux*, Result> Linux::New(const System::String& title) {
    struct wl_display* display = wl_display_connect(nullptr);
    if (display == nullptr) {
        Panic("wl_display_connect failed");
    }

    struct wl_registry* registry = wl_display_get_registry(display);
    if (registry == nullptr) {
        Panic("wl_display_get_registry failed");
    }

    Linux* _linux = new Linux(display, registry);

    int result = wl_registry_add_listener(_linux->registry, &Linux::RegistryListener, _linux);
    CELL_ASSERT(result == 0);

    _linux->keyboardContext = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    CELL_ASSERT(_linux->keyboardContext != nullptr);

    result = wl_display_roundtrip(_linux->display);
    CELL_ASSERT(result > -1);

    CELL_ASSERT(_linux->compositor != nullptr && _linux->seat != nullptr && _linux->xdgManager != nullptr);

    _linux->surface = wl_compositor_create_surface(_linux->compositor);
    if (_linux->surface == nullptr) {
        Panic("wl_compositor_create_surface failed");
    }

    _linux->xdgSurface = xdg_wm_base_get_xdg_surface(_linux->xdgManager, _linux->surface);
    if (_linux->xdgSurface == nullptr) {
        Panic("xdg_wm_base_get_xdg_surface failed");
    }

    result = xdg_surface_add_listener(_linux->xdgSurface, &Linux::XDGSurfaceListener, _linux);
    CELL_ASSERT(result == 0);

    _linux->xdgToplevel = xdg_surface_get_toplevel(_linux->xdgSurface);
    if (_linux->xdgToplevel == nullptr) {
        Panic("xdg_surface_get_toplevel failed");
    }

    result = xdg_toplevel_add_listener(_linux->xdgToplevel, &Linux::XDGToplevelListener, _linux);
    CELL_ASSERT(result == 0);

    if (_linux->xdgDecorationManager != nullptr) {
        _linux->xdgDecoration = zxdg_decoration_manager_v1_get_toplevel_decoration(_linux->xdgDecorationManager, _linux->xdgToplevel);
        if (_linux->xdgDecoration == nullptr) {
            Panic("zxdg_decoration_manager_v1_get_toplevel_decoration failed");
        }

        result = zxdg_toplevel_decoration_v1_add_listener(_linux->xdgDecoration, &Linux::XDGDecorationListener, _linux);
        CELL_ASSERT(result == 0);

        zxdg_toplevel_decoration_v1_set_mode(_linux->xdgDecoration, ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);
    } else {
        Log("zxdg_decoration_manager_v1 is unavailable, decorations might break or not show up");
    }

    if (_linux->idleInhibitManager != nullptr) {
        _linux->idleInhibitor = zwp_idle_inhibit_manager_v1_create_inhibitor(_linux->idleInhibitManager, _linux->surface);
        if (_linux->idleInhibitor == nullptr) {
            Panic("zwp_idle_inhibit_manager_v1_create_inhibitor failed");
        }
    } else {
        Log("zwp_idle_inhibit_manager_v1 is unavailable");
    }

    if (_linux->cursorShapeManager != nullptr) {
        _linux->cursorShapeDevice = wp_cursor_shape_manager_v1_get_pointer(_linux->cursorShapeManager, _linux->pointer);
        if (_linux->cursorShapeDevice == nullptr) {
            Panic("wp_cursor_shape_manager_v1_get_pointer failed");
        }
    } else {
        Log("wp_cursor_shape_manager_v1 is unavailable");
    }

    char* titleStr = title.IsEmpty() ? (char*)"Cell" : title.ToCharPointer();
    xdg_toplevel_set_title(_linux->xdgToplevel, titleStr);
    xdg_toplevel_set_app_id(_linux->xdgToplevel, titleStr);

    _linux->SetDrawableExtent({ 1280, 720 });

    if (!title.IsEmpty()) {
        System::FreeMemory(titleStr);
    }

    return _linux;
}

Linux::~Linux() {
    if (this->cursorShapeDevice != nullptr) {
        wp_cursor_shape_device_v1_destroy(this->cursorShapeDevice);
    }

    if (this->idleInhibitor != nullptr) {
        zwp_idle_inhibitor_v1_destroy(this->idleInhibitor);
    }

    if (this->xdgDecoration != nullptr) {
        zxdg_toplevel_decoration_v1_destroy(this->xdgDecoration);
    }

    if (this->pointer != nullptr) {
        wl_pointer_destroy(this->pointer);
    }

    if (this->keyboard != nullptr) {
        xkb_state_unref(this->keyboardState);
        xkb_keymap_unref(this->keyboardKeymap);
        xkb_context_unref(this->keyboardContext);
        wl_keyboard_destroy(this->keyboard);
    }

    xdg_toplevel_destroy(this->xdgToplevel);
    xdg_surface_destroy(this->xdgSurface);
    wl_surface_destroy(this->surface);

    if (this->cursorShapeManager != nullptr) {
        wp_cursor_shape_manager_v1_destroy(this->cursorShapeManager);
    }

    if (this->idleInhibitManager != nullptr) {
        zwp_idle_inhibit_manager_v1_destroy(this->idleInhibitManager);
    }

    if (this->xdgDecorationManager != nullptr) {
        zxdg_decoration_manager_v1_destroy(this->xdgDecorationManager);
    }

    xdg_wm_base_destroy(this->xdgManager);
    wl_seat_destroy(this->seat);
    wl_compositor_destroy(this->compositor);

    wl_registry_destroy(this->registry);
    wl_display_disconnect(this->display);
}

}
