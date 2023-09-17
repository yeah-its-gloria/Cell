// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/Linux.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Memory.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Sleep.hh>

namespace Cell::Shell::Implementations {
using namespace System;

Linux::~Linux() {
    if (this->idleInhibitor != nullptr) {
        zwp_idle_inhibitor_v1_destroy(this->idleInhibitor);
    }

    if (this->xdgDecoration != nullptr) {
        zxdg_toplevel_decoration_v1_destroy(this->xdgDecoration);
    }

    xdg_toplevel_destroy(this->xdgToplevel);
    xdg_surface_destroy(this->xdgSurface);
    wl_surface_destroy(this->surface);

    if (this->idleInhibitManager != nullptr) {
        zwp_idle_inhibit_manager_v1_destroy(this->idleInhibitManager);
    }

    if (this->xdgDecorationManager != nullptr) {
        zxdg_decoration_manager_v1_destroy(this->xdgDecorationManager);
    }

    xdg_wm_base_destroy(this->xdgManager);
    wl_compositor_destroy(this->compositor);

    wl_registry_destroy(this->registry);
    wl_display_disconnect(this->display);
}

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

    int waylandResult = wl_registry_add_listener(_linux->registry, &Linux::RegistryListener, _linux);
    CELL_ASSERT(waylandResult == 0);

    _linux->keyboardContext = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    CELL_ASSERT(_linux->keyboardContext != nullptr);

    waylandResult = wl_display_roundtrip(_linux->display);
    CELL_ASSERT(waylandResult > -1);

    CELL_ASSERT(_linux->compositor != nullptr && _linux->seat != nullptr && _linux->xdgManager != nullptr);

    _linux->surface = wl_compositor_create_surface(_linux->compositor);
    if (_linux->surface == nullptr) {
        Panic("wl_compositor_create_surface failed");
    }

    _linux->xdgSurface = xdg_wm_base_get_xdg_surface(_linux->xdgManager, _linux->surface);
    if (_linux->xdgSurface == nullptr) {
        Panic("xdg_wm_base_get_xdg_surface failed");
    }

    waylandResult = xdg_surface_add_listener(_linux->xdgSurface, &Linux::SurfaceListener, _linux);
    CELL_ASSERT(waylandResult == 0);

    _linux->xdgToplevel = xdg_surface_get_toplevel(_linux->xdgSurface);
    if (_linux->xdgToplevel == nullptr) {
        Panic("xdg_surface_get_toplevel failed");
    }

    waylandResult = xdg_toplevel_add_listener(_linux->xdgToplevel, &Linux::ToplevelListener, _linux);
    CELL_ASSERT(waylandResult == 0);

    if (_linux->xdgDecorationManager != nullptr) {
        _linux->xdgDecoration = zxdg_decoration_manager_v1_get_toplevel_decoration(_linux->xdgDecorationManager, _linux->xdgToplevel);
        if (_linux->xdgDecoration == nullptr) {
            Panic("zxdg_decoration_manager_v1_get_toplevel_decoration failed");
        }

        waylandResult = zxdg_toplevel_decoration_v1_add_listener(_linux->xdgDecoration, &Linux::DecorationListener, _linux);
        CELL_ASSERT(waylandResult == 0);

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

    char* titleStr = title.IsEmpty() ? (char*)"Cell" : title.ToCharPointer();
    xdg_toplevel_set_title(_linux->xdgToplevel, titleStr);
    xdg_toplevel_set_app_id(_linux->xdgToplevel, titleStr);
    xdg_surface_set_window_geometry(_linux->xdgSurface, 0, 0, 1280, 720);

    wl_surface_commit(_linux->surface);

    if (!title.IsEmpty()) {
        System::FreeMemory(titleStr);
    }

    return _linux;
}

Result Linux::RunDispatch() {
    const int waylandResult = wl_display_dispatch_pending(this->display);
    CELL_ASSERT(waylandResult > -1);

    if (this->xdgRequestedClose) {
        return Result::RequestedQuit;
    }

    return Result::Success;
}

}
