// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Log.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Sleep.hh>
#include <Cell/System/Platform/Linux.hh>

namespace Cell::System::Platform {

struct trampolineData {
    const EntryFunction entry;
    Reference<IPlatform> platform;
    Reference<String> parameterString;
};

CELL_FUNCTION_INTERNAL void cellEntryTrampoline(void* data) {
    const struct trampolineData* entryData = (const struct trampolineData*)data;
    CELL_ASSERT(entryData != nullptr);

    entryData->entry(entryData->platform, entryData->parameterString);
}

Linux::Linux() {
    this->mainThread = nullptr;
    this->readySignal = new Event();
    this->exitSignal = new Event();

    this->xdgCloseSignal = new Event();
    this->keyLock = new Mutex();
}

Linux::~Linux() {
    delete this->mainThread;

    delete this->xdgCloseSignal;
    delete this->exitSignal;
    delete this->readySignal;
}

void Linux::Start(const EntryFunction entry, const bool needsWindow) {
    Linux _linux;

    String parameters = "";

    const struct trampolineData trampolineData = {
        .entry           = entry,
        .platform        = Reference<IPlatform>(_linux),
        .parameterString = Reference(parameters)
    };

    _linux.mainThread = new Thread((const ThreadFunction)cellEntryTrampoline, (void*)&trampolineData, "Title Thread");

    // set up wayland

    int waylandResult;
    if (needsWindow) {
        _linux.display = wl_display_connect(nullptr);
        if (_linux.display == nullptr) {
            Panic("wl_display_connect failed");
        }

        _linux.registry = wl_display_get_registry(_linux.display);
        if (_linux.registry == nullptr) {
            Panic("wl_display_get_registry failed");
        }

        const struct wl_registry_listener registryListener =
            {
                .global        = &Linux::WaylandRegistryAdd,
                .global_remove = &Linux::WaylandRegistryRemove
            };

        waylandResult = wl_registry_add_listener(_linux.registry, &registryListener, &_linux);
        CELL_ASSERT(waylandResult == 0);

        _linux.keyboardContext = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
        CELL_ASSERT(_linux.keyboardContext != nullptr);

        waylandResult = wl_display_roundtrip(_linux.display);
        CELL_ASSERT(waylandResult > -1);

        CELL_ASSERT(_linux.compositor != nullptr && _linux.seat != nullptr && _linux.xdgManager != nullptr);

        _linux.surface = wl_compositor_create_surface(_linux.compositor);
        if (_linux.surface == nullptr) {
            Panic("wl_compositor_create_surface failed");
        }

        _linux.xdgSurface = xdg_wm_base_get_xdg_surface(_linux.xdgManager, _linux.surface);
        if (_linux.xdgSurface == nullptr) {
            Panic("xdg_wm_base_get_xdg_surface failed");
        }

        const struct xdg_surface_listener surfaceListener =
            {
                .configure = &Linux::WaylandXDGSurfaceConfigure,
            };

        waylandResult = xdg_surface_add_listener(_linux.xdgSurface, &surfaceListener, &_linux);
        CELL_ASSERT(waylandResult == 0);

        _linux.xdgToplevel = xdg_surface_get_toplevel(_linux.xdgSurface);
        if (_linux.xdgToplevel == nullptr) {
            Panic("xdg_surface_get_toplevel failed");
        }

        const struct xdg_toplevel_listener toplevelListener =
            {
                .configure        = &Linux::WaylandXDGToplevelConfigure,
                .close            = &Linux::WaylandXDGToplevelClose,
                .configure_bounds = &Linux::WaylandXDGToplevelConfigureBounds,
                .wm_capabilities  = &Linux::WaylandXDGToplevelAnnounceWMCapabilities
            };

        waylandResult = xdg_toplevel_add_listener(_linux.xdgToplevel, &toplevelListener, &_linux);
        CELL_ASSERT(waylandResult == 0);

        xdg_toplevel_set_title(_linux.xdgToplevel, "Cell");
        xdg_toplevel_set_app_id(_linux.xdgToplevel, "cell");

        if (_linux.xdgDecorationManager != nullptr) {
            _linux.xdgDecoration = zxdg_decoration_manager_v1_get_toplevel_decoration(_linux.xdgDecorationManager, _linux.xdgToplevel);
            if (_linux.xdgDecoration == nullptr) {
                Panic("zxdg_decoration_manager_v1_get_toplevel_decoration failed");
            }

            const struct zxdg_toplevel_decoration_v1_listener decorationListener =
                {
                    .configure = &Linux::WaylandXDGDecorationConfigure,
                };

            waylandResult = zxdg_toplevel_decoration_v1_add_listener(_linux.xdgDecoration, &decorationListener, &_linux);
            CELL_ASSERT(waylandResult == 0);

            zxdg_toplevel_decoration_v1_set_mode(_linux.xdgDecoration, ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);
        } else {
            Log("zxdg_decoration_manager_v1 is unavailable, decorations might break or not show up");
        }

        if (_linux.idleInhibitManager != nullptr) {
            _linux.idleInhibitor = zwp_idle_inhibit_manager_v1_create_inhibitor(_linux.idleInhibitManager, _linux.surface);
            if (_linux.idleInhibitor == nullptr) {
                Panic("zwp_idle_inhibit_manager_v1_create_inhibitor failed");
            }
        } else {
            Log("zwp_idle_inhibit_manager_v1 is unavailable");
        }

        xdg_surface_set_window_geometry(_linux.xdgSurface, 0, 0, 1280, 720);

        wl_surface_commit(_linux.surface);
    }

    // Signal that the platform is ready

    _linux.readySignal->Signal();

    // Management loop

    while (_linux.mainThread->IsActive()) {
        if (needsWindow) {
            waylandResult = wl_display_dispatch_pending(_linux.display);
            CELL_ASSERT(waylandResult > -1);
        }

        if (_linux.xdgCloseSignal->Wait(1) == EventWaitState::Signaled) {
            break;
        }

        Thread::Yield();
    }

    // Wait for the main thread to act on the exit signal

    _linux.exitSignal->Signal();
    _linux.mainThread->Join();

    if (needsWindow) {
        if (_linux.idleInhibitor != nullptr) {
            zwp_idle_inhibitor_v1_destroy(_linux.idleInhibitor);
        }

        if (_linux.xdgDecoration != nullptr) {
            zxdg_toplevel_decoration_v1_destroy(_linux.xdgDecoration);
        }

        xdg_toplevel_destroy(_linux.xdgToplevel);
        xdg_surface_destroy(_linux.xdgSurface);
        wl_surface_destroy(_linux.surface);

        if (_linux.idleInhibitManager != nullptr) {
            zwp_idle_inhibit_manager_v1_destroy(_linux.idleInhibitManager);
        }

        if (_linux.xdgDecorationManager != nullptr) {
            zxdg_decoration_manager_v1_destroy(_linux.xdgDecorationManager);
        }

        xdg_wm_base_destroy(_linux.xdgManager);
        wl_compositor_destroy(_linux.compositor);

        wl_registry_destroy(_linux.registry);
        wl_display_disconnect(_linux.display);
    }
}

}
