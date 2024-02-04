// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Shell/Shell.hh>
#include <Cell/System/Mutex.hh>

#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>

#include <Cell/Shell/Implementations/Linux/Protocols/cursor-shape-client-protocol.h>
#include <Cell/Shell/Implementations/Linux/Protocols/idle-inhibit-client-protocol.h>
#include <Cell/Shell/Implementations/Linux/Protocols/xdg-decoration-client-protocol.h>
#include <Cell/Shell/Implementations/Linux/Protocols/xdg-shell-client-protocol.h>

namespace Cell::Shell::Implementations {

// Linux shell implementation using Wayland as a backend.
class Linux : public IShell {
public:
    // Sets up a Wayland client.
    CELL_FUNCTION static Wrapped<Linux*, Result> New(const System::String& title = "");

    // Terminates the Wayland client connection and destructs the instance.
    CELL_FUNCTION ~Linux() override;

    // Returns a handle to the current Wayland client display.
    CELL_NODISCARD CELL_INLINE struct wl_display* GetDisplay() const { return this->display; }

    // Returns a handle to the current Wayland client surface.
    CELL_NODISCARD CELL_INLINE struct wl_surface* GetSurface() const { return this->surface; }

    CELL_FUNCTION Result RequestQuit() override;
    CELL_FUNCTION Wrapped<Extent, Result> GetDrawableExtent() override;
    CELL_FUNCTION Result SetDrawableExtent(const Extent extent) override;
    CELL_FUNCTION Result SetNewTitle(const System::String& title) override;
    CELL_FUNCTION Result IndicateStatus(const ShellStatus status) override;

private:
    CELL_INLINE Linux(struct wl_display* display, struct wl_registry* registry) : display(display), registry(registry) { }

    CELL_FUNCTION_INTERNAL Result RunDispatchImpl() override;

    Extent extent = { 0, 0 };

    // Core

    CELL_FUNCTION_INTERNAL static void RegistryAdd(void* data, struct wl_registry* registry, const uint32_t name, const char* interface, const uint32_t version);
    CELL_FUNCTION_INTERNAL static void RegistryRemove(void* data, struct wl_registry* registry, const uint32_t name);

    static constexpr struct wl_registry_listener RegistryListener = {
        .global = &Linux::RegistryAdd,
        .global_remove = &Linux::RegistryRemove
    };

    struct wl_display* display;
    struct wl_registry* registry;

    struct wl_compositor* compositor = nullptr;
    struct wl_surface* surface = nullptr;

    // Seat

    CELL_FUNCTION_INTERNAL static void SeatCapabilities(void* data, struct wl_seat* seat, const uint32_t capabilities);
    CELL_FUNCTION_INTERNAL static void SeatName(void* data, struct wl_seat* seat, const char* name);

    CELL_FUNCTION_INTERNAL static void KeyboardKeymap(void* data, struct wl_keyboard* keyboard, const uint32_t format, const int32_t fd, const uint32_t size);
    CELL_FUNCTION_INTERNAL static void KeyboardEnter(void* data, struct wl_keyboard* keyboard, uint32_t serial, struct wl_surface* surface, struct wl_array* keys);
    CELL_FUNCTION_INTERNAL static void KeyboardLeave(void* data, struct wl_keyboard* keyboard, uint32_t serial, struct wl_surface* surface);
    CELL_FUNCTION_INTERNAL static void KeyboardKey(void* data, struct wl_keyboard* keyboard, const uint32_t serial, const uint32_t time, const uint32_t key, const uint32_t state);
    CELL_FUNCTION_INTERNAL static void KeyboardRepeatInfo(void* data, struct wl_keyboard* keyboard, const int32_t rate, const int32_t delay);

    CELL_FUNCTION_INTERNAL static void KeyboardModifiers(void* data,
                                                         struct wl_keyboard* keyboard,
                                                         const uint32_t serial,
                                                         const uint32_t modsDepressed,
                                                         const uint32_t modsLatched,
                                                         const uint32_t modsLocked,
                                                         const uint32_t group);

    CELL_FUNCTION_INTERNAL static void PointerEnter(void* data, struct wl_pointer* pointer, const uint32_t serial, struct wl_surface* surface, const wl_fixed_t x, const wl_fixed_t y);
    CELL_FUNCTION_INTERNAL static void PointerLeave(void* data, struct wl_pointer* pointer, const uint32_t serial, struct wl_surface* surface);
    CELL_FUNCTION_INTERNAL static void PointerMotion(void* data, struct wl_pointer* pointer, const uint32_t time, const wl_fixed_t x, const wl_fixed_t y);
    CELL_FUNCTION_INTERNAL static void PointerButton(void* data, struct wl_pointer* pointer, const uint32_t serial, const uint32_t time, const uint32_t button, const uint32_t state);
    CELL_FUNCTION_INTERNAL static void PointerAxis(void* data, struct wl_pointer* pointer, const uint32_t time, const uint32_t axis, const wl_fixed_t value);
    CELL_FUNCTION_INTERNAL static void PointerFrameEnd(void* data, struct wl_pointer* pointer);
    CELL_FUNCTION_INTERNAL static void PointerAxisStop(void* data, struct wl_pointer* pointer, const uint32_t time, const uint32_t axis);

    static constexpr struct wl_seat_listener SeatListener = {
        .capabilities = &Linux::SeatCapabilities,
        .name = &Linux::SeatName
    };

    static constexpr struct wl_keyboard_listener KeyboardListener = {
        .keymap = &Linux::KeyboardKeymap,
        .enter = &Linux::KeyboardEnter,
        .leave = &Linux::KeyboardLeave,
        .key = &Linux::KeyboardKey,
        .modifiers = &Linux::KeyboardModifiers,
        .repeat_info = &Linux::KeyboardRepeatInfo
    };

    static constexpr struct wl_pointer_listener PointerListener = {
        .enter = &Linux::PointerEnter,
        .leave = &Linux::PointerLeave,
        .motion = &Linux::PointerMotion,
        .button = &Linux::PointerButton,
        .axis = &Linux::PointerAxis,
        .frame = &Linux::PointerFrameEnd,
        .axis_source = nullptr,
        .axis_stop = &Linux::PointerAxisStop,
        .axis_discrete = nullptr,
#ifdef WL_POINTER_AXIS_VALUE120_SINCE_VERSION
        .axis_value120 = nullptr,
#endif
#ifdef WL_POINTER_AXIS_RELATIVE_DIRECTION_SINCE_VERSION
        .axis_relative_direction = nullptr
#endif
    };

    struct wl_seat* seat = nullptr;

    struct wl_keyboard* keyboard = nullptr;
    struct xkb_context* keyboardContext = nullptr;
    struct xkb_keymap* keyboardKeymap = nullptr;
    struct xkb_state* keyboardState = nullptr;
    bool keyboardActive = false;

    struct wl_pointer* pointer = nullptr;
    bool pointerActive = false;
    uint32_t pointerSerial = 0;

    // XDG

    CELL_FUNCTION_INTERNAL static void XDGManagerPing(void* data, struct xdg_wm_base* manager, const uint32_t serial);

    CELL_FUNCTION_INTERNAL static void XDGSurfaceConfigure(void* data, struct xdg_surface* surface, const uint32_t serial);

    CELL_FUNCTION_INTERNAL static void XDGToplevelConfigure(void* data, struct xdg_toplevel* toplevel, const int width, const int height, struct wl_array* states);
    CELL_FUNCTION_INTERNAL static void XDGToplevelClose(void* data, struct xdg_toplevel* toplevel);
    CELL_FUNCTION_INTERNAL static void XDGToplevelConfigureBounds(void* data, struct xdg_toplevel* toplevel, const int32_t width, const int32_t height);
    CELL_FUNCTION_INTERNAL static void XDGToplevelAnnounceWMCapabilities(void* data, struct xdg_toplevel* toplevel, struct wl_array* capabilities);

    CELL_FUNCTION_INTERNAL static void XDGDecorationConfigure(void* data, struct zxdg_toplevel_decoration_v1* decoration, const uint32_t mode);

    static constexpr struct xdg_wm_base_listener XDGManagerListener = {
        .ping = &Linux::XDGManagerPing
    };

    static constexpr struct xdg_surface_listener XDGSurfaceListener = {
        .configure = &Linux::XDGSurfaceConfigure,
    };

    static constexpr struct xdg_toplevel_listener XDGToplevelListener = {
        .configure = &Linux::XDGToplevelConfigure,
        .close = &Linux::XDGToplevelClose,
        .configure_bounds = &Linux::XDGToplevelConfigureBounds,
        .wm_capabilities = &Linux::XDGToplevelAnnounceWMCapabilities
    };

    static constexpr struct zxdg_toplevel_decoration_v1_listener XDGDecorationListener = {
        .configure = &Linux::XDGDecorationConfigure,
    };

    struct xdg_wm_base* xdgManager = nullptr;

    struct xdg_surface* xdgSurface = nullptr;
    struct xdg_toplevel* xdgToplevel = nullptr;

    struct zxdg_decoration_manager_v1* xdgDecorationManager = nullptr;
    struct zxdg_toplevel_decoration_v1* xdgDecoration = nullptr;

    // Extensions

    struct zwp_idle_inhibit_manager_v1* idleInhibitManager = nullptr;
    struct zwp_idle_inhibitor_v1* idleInhibitor = nullptr;

    struct wp_cursor_shape_manager_v1* cursorShapeManager = nullptr;
    struct wp_cursor_shape_device_v1* cursorShapeDevice = nullptr;
};

}
