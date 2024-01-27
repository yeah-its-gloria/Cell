// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Shell/Shell.hh>
#include <Cell/System/Mutex.hh>

#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>

#include <Cell/Shell/Implementations/Linux/WaylandProtocols/idle-inhibit-client-protocol.h>
#include <Cell/Shell/Implementations/Linux/WaylandProtocols/xdg-decoration-client-protocol.h>
#include <Cell/Shell/Implementations/Linux/WaylandProtocols/xdg-shell-client-protocol.h>

namespace Cell::Shell::Implementations {

// Platform implementation for Linux. Should support both glibc and musl.
class Linux : public IShell {
public:
    // Creates a new shell instance.
    CELL_FUNCTION static Wrapped<Linux*, Result> New(const System::String& title = "");

    // Destructs the shell instance.
    CELL_FUNCTION ~Linux() override;

    // Returns a handle to the current Wayland display.
    CELL_NODISCARD CELL_INLINE struct wl_display* GetWaylandDisplay() const { return this->display; }

    // Returns a handle to the current Wayland surface.
    CELL_NODISCARD CELL_INLINE struct wl_surface* GetWaylandSurface() const { return this->surface; }

    // Returns a handle to the current XDG toplevel instance.
    CELL_NODISCARD CELL_INLINE struct xdg_toplevel* GetWaylandXDGToplevel() const { return this->xdgToplevel; }

    CELL_FUNCTION Result RequestQuit() override;
    CELL_FUNCTION Wrapped<Extent, Result> GetDrawableExtent() override;
    CELL_FUNCTION Result SetDrawableExtent(const Extent extent) override;
    CELL_FUNCTION Result SetNewTitle(const System::String& title) override;
    CELL_FUNCTION Result IndicateStatus(const ShellStatus status) override;

private:
    CELL_INLINE Linux(struct wl_display* display, struct wl_registry* registry) : display(display), registry(registry) { }

    CELL_FUNCTION_INTERNAL Result RunDispatchImpl() override;

    CELL_FUNCTION_INTERNAL static void WaylandXDGManagerPing(void* data, struct xdg_wm_base* manager, const uint32_t serial);
    CELL_FUNCTION_INTERNAL static void WaylandXDGSurfaceConfigure(void* data, struct xdg_surface* surface, const uint32_t serial);
    CELL_FUNCTION_INTERNAL static void WaylandXDGDecorationConfigure(void* data, struct zxdg_toplevel_decoration_v1* decoration, const uint32_t mode);

    CELL_FUNCTION_INTERNAL static void WaylandRegistryAdd(void* data, struct wl_registry* registry, const uint32_t name, const char* interface, const uint32_t version);
    CELL_FUNCTION_INTERNAL static void WaylandRegistryRemove(void* data, struct wl_registry* registry, const uint32_t name);

    CELL_FUNCTION_INTERNAL static void WaylandSeatCapabilities(void* data, struct wl_seat* seat, const uint32_t capabilities);
    CELL_FUNCTION_INTERNAL static void WaylandSeatName(void* data, struct wl_seat* seat, const char* name);

    CELL_FUNCTION_INTERNAL static void WaylandKeyboardKeymap(void* data, struct wl_keyboard* keyboard, const uint32_t format, const int32_t fd, const uint32_t size);
    CELL_FUNCTION_INTERNAL static void WaylandKeyboardEnter(void* data, struct wl_keyboard* keyboard, uint32_t serial, struct wl_surface* surface, struct wl_array* keys);
    CELL_FUNCTION_INTERNAL static void WaylandKeyboardLeave(void* data, struct wl_keyboard* keyboard, uint32_t serial, struct wl_surface* surface);
    CELL_FUNCTION_INTERNAL static void WaylandKeyboardKey(void* data, struct wl_keyboard* keyboard, const uint32_t serial, const uint32_t time, const uint32_t key, const uint32_t state);
    CELL_FUNCTION_INTERNAL static void WaylandKeyboardRepeatInfo(void* data, struct wl_keyboard* keyboard, const int32_t rate, const int32_t delay);

    CELL_FUNCTION_INTERNAL static void WaylandKeyboardModifiers(void* data,
                                                                struct wl_keyboard* keyboard,
                                                                const uint32_t serial,
                                                                const uint32_t modsDepressed,
                                                                const uint32_t modsLatched,
                                                                const uint32_t modsLocked,
                                                                const uint32_t group);

    CELL_FUNCTION_INTERNAL static void WaylandXDGToplevelConfigure(void* data, struct xdg_toplevel* toplevel, const int width, const int height, struct wl_array* states);
    CELL_FUNCTION_INTERNAL static void WaylandXDGToplevelClose(void* data, struct xdg_toplevel* toplevel);
    CELL_FUNCTION_INTERNAL static void WaylandXDGToplevelConfigureBounds(void* data, struct xdg_toplevel* toplevel, const int32_t width, const int32_t height);
    CELL_FUNCTION_INTERNAL static void WaylandXDGToplevelAnnounceWMCapabilities(void* data, struct xdg_toplevel* toplevel, struct wl_array* capabilities);

    struct wl_display* display;
    struct wl_registry* registry;

    struct wl_seat* seat = nullptr;
    struct wl_pointer* pointer = nullptr;

    struct wl_keyboard* keyboard = nullptr;
    struct xkb_context* keyboardContext = nullptr;
    struct xkb_keymap* keyboardKeymap = nullptr;
    struct xkb_state* keyboardState = nullptr;

    struct xdg_wm_base* xdgManager = nullptr;
    struct wl_compositor* compositor = nullptr;
    struct wl_surface* surface = nullptr;
    struct xdg_surface* xdgSurface = nullptr;
    struct xdg_toplevel* xdgToplevel = nullptr;
    struct zxdg_decoration_manager_v1* xdgDecorationManager = nullptr;
    struct zxdg_toplevel_decoration_v1* xdgDecoration = nullptr;
    struct zwp_idle_inhibit_manager_v1* idleInhibitManager = nullptr;
    struct zwp_idle_inhibitor_v1* idleInhibitor = nullptr;

    static constexpr struct wl_registry_listener RegistryListener = {
        .global = &Linux::WaylandRegistryAdd,
        .global_remove = &Linux::WaylandRegistryRemove
    };

    static constexpr struct xdg_surface_listener SurfaceListener = {
        .configure = &Linux::WaylandXDGSurfaceConfigure,
    };

    static constexpr struct xdg_toplevel_listener ToplevelListener = {
        .configure = &Linux::WaylandXDGToplevelConfigure,
        .close = &Linux::WaylandXDGToplevelClose,
        .configure_bounds = &Linux::WaylandXDGToplevelConfigureBounds,
        .wm_capabilities = &Linux::WaylandXDGToplevelAnnounceWMCapabilities
    };

    static constexpr struct zxdg_toplevel_decoration_v1_listener DecorationListener = {
        .configure = &Linux::WaylandXDGDecorationConfigure,
    };

    static constexpr struct wl_seat_listener SeatListener = {
        .capabilities = &Linux::WaylandSeatCapabilities,
        .name = &Linux::WaylandSeatName
    };

    static constexpr struct xdg_wm_base_listener XDGManagerListener = {
        .ping = &Linux::WaylandXDGManagerPing
    };

    static constexpr struct wl_keyboard_listener KeyboardListener = {
        .keymap = &Linux::WaylandKeyboardKeymap,
        .enter = &Linux::WaylandKeyboardEnter,
        .leave = &Linux::WaylandKeyboardLeave,
        .key = &Linux::WaylandKeyboardKey,
        .modifiers = &Linux::WaylandKeyboardModifiers,
        .repeat_info = &Linux::WaylandKeyboardRepeatInfo
    };
};

}
