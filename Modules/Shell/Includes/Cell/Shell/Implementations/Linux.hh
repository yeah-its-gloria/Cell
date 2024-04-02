// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Shell/Shell.hh>
#include <Cell/System/Mutex.hh>

#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>

#include <Cell/Shell/Implementations/Linux/Protocols/cursor-shape-client-protocol.h>
#include <Cell/Shell/Implementations/Linux/Protocols/idle-inhibit-client-protocol.h>
#include <Cell/Shell/Implementations/Linux/Protocols/relative-pointer-client-protocol.h>
#include <Cell/Shell/Implementations/Linux/Protocols/pointer-constraints-client-protocol.h>
#include <Cell/Shell/Implementations/Linux/Protocols/xdg-decoration-client-protocol.h>
#include <Cell/Shell/Implementations/Linux/Protocols/xdg-shell-client-protocol.h>

namespace Cell::Shell::Implementations {

// Linux shell implementation using Wayland as a backend.
class Linux : public IShell {
public:
    // Sets up a Wayland client.
    CELL_FUNCTION static Wrapped<Linux*, Result> New(const String& title, const Extent extent);

    CELL_FUNCTION ~Linux();

    CELL_NODISCARD CELL_FUNCTION_TEMPLATE wl_display* GetDisplay() const { return this->display; }
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE wl_surface* GetSurface() const { return this->surface; }

    CELL_FUNCTION Result RequestQuit() override;
    CELL_FUNCTION Wrapped<Extent, Result> GetDrawableExtent() override;
    CELL_FUNCTION Result SetDrawableExtent(const Extent extent) override;
    CELL_FUNCTION Result SetNewTitle(const String& title) override;
    CELL_FUNCTION Result IndicateStatus(const ShellStatus status) override;
    CELL_FUNCTION Result CaptureState(const bool captured) override;
    CELL_FUNCTION Result Log(const String& text) override;
    CELL_FUNCTION Result LogClear() override;

private:
    CELL_FUNCTION_INTERNAL Linux(wl_display* display, wl_registry* registry) : display(display), registry(registry) { }

    CELL_FUNCTION_INTERNAL Result RunDispatchImpl() override;

    Extent extent = { 0, 0 };

    // Core

    CELL_FUNCTION_INTERNAL static void RegistryAdd(void* data, wl_registry* registry, const uint32_t name, const char* interface, const uint32_t version);
    CELL_FUNCTION_INTERNAL static void RegistryRemove(void* data, wl_registry* registry, const uint32_t name);

    static constexpr wl_registry_listener RegistryListener = {
        .global = &Linux::RegistryAdd,
        .global_remove = &Linux::RegistryRemove
    };

    wl_display* display;
    wl_registry* registry;

    wl_compositor* compositor = nullptr;
    wl_surface* surface = nullptr;

    // Seat

    CELL_FUNCTION_INTERNAL static void SeatCapabilities(void* data, wl_seat* seat, const uint32_t capabilities);
    CELL_FUNCTION_INTERNAL static void SeatName(void* data, wl_seat* seat, const char* name);

    CELL_FUNCTION_INTERNAL static void KeyboardKeymap(void* data, wl_keyboard* keyboard, const uint32_t format, const int32_t fd, const uint32_t size);
    CELL_FUNCTION_INTERNAL static void KeyboardEnter(void* data, wl_keyboard* keyboard, uint32_t serial, wl_surface* surface, wl_array* keys);
    CELL_FUNCTION_INTERNAL static void KeyboardLeave(void* data, wl_keyboard* keyboard, uint32_t serial, wl_surface* surface);
    CELL_FUNCTION_INTERNAL static void KeyboardKey(void* data, wl_keyboard* keyboard, const uint32_t serial, const uint32_t time, const uint32_t key, const uint32_t state);
    CELL_FUNCTION_INTERNAL static void KeyboardRepeatInfo(void* data, wl_keyboard* keyboard, const int32_t rate, const int32_t delay);

    CELL_FUNCTION_INTERNAL static void KeyboardModifiers(void* data,
                                                         wl_keyboard* keyboard,
                                                         const uint32_t serial,
                                                         const uint32_t modsDepressed,
                                                         const uint32_t modsLatched,
                                                         const uint32_t modsLocked,
                                                         const uint32_t group);

    CELL_FUNCTION_INTERNAL static void PointerEnter(void* data, wl_pointer* pointer, const uint32_t serial, wl_surface* surface, const wl_fixed_t x, const wl_fixed_t y);
    CELL_FUNCTION_INTERNAL static void PointerLeave(void* data, wl_pointer* pointer, const uint32_t serial, wl_surface* surface);
    CELL_FUNCTION_INTERNAL static void PointerMotion(void* data, wl_pointer* pointer, const uint32_t time, const wl_fixed_t x, const wl_fixed_t y);
    CELL_FUNCTION_INTERNAL static void PointerButton(void* data, wl_pointer* pointer, const uint32_t serial, const uint32_t time, const uint32_t button, const uint32_t state);
    CELL_FUNCTION_INTERNAL static void PointerAxis(void* data, wl_pointer* pointer, const uint32_t time, const uint32_t axis, const wl_fixed_t value);
    CELL_FUNCTION_INTERNAL static void PointerFrameEnd(void* data, wl_pointer* pointer);
    CELL_FUNCTION_INTERNAL static void PointerAxisStop(void* data, wl_pointer* pointer, const uint32_t time, const uint32_t axis);

    CELL_FUNCTION_INTERNAL static void PointerLocked(void* data, zwp_locked_pointer_v1* pointer);
    CELL_FUNCTION_INTERNAL static void PointerUnlocked(void* data, zwp_locked_pointer_v1* pointer);

    CELL_FUNCTION_INTERNAL static void PointerRelativeMotion(void* data,
                                                             zwp_relative_pointer_v1* pointer,
                                                             uint32_t timestampHigh,
                                                             uint32_t timestampLow,
                                                             const wl_fixed_t x,
                                                             const wl_fixed_t y,
                                                             const wl_fixed_t xUnaccel,
                                                             const wl_fixed_t yUnaccel);

    static constexpr wl_seat_listener SeatListener = {
        .capabilities = &Linux::SeatCapabilities,
        .name = &Linux::SeatName
    };

    static constexpr wl_keyboard_listener KeyboardListener = {
        .keymap = &Linux::KeyboardKeymap,
        .enter = &Linux::KeyboardEnter,
        .leave = &Linux::KeyboardLeave,
        .key = &Linux::KeyboardKey,
        .modifiers = &Linux::KeyboardModifiers,
        .repeat_info = &Linux::KeyboardRepeatInfo
    };

    static constexpr wl_pointer_listener PointerListener = {
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

    static constexpr zwp_locked_pointer_v1_listener LockedPointerListener = {
        .locked = &Linux::PointerLocked,
        .unlocked = &Linux::PointerUnlocked,
    };

    static constexpr zwp_relative_pointer_v1_listener RelativePointerListener = {
        .relative_motion = &Linux::PointerRelativeMotion,
    };

    wl_seat* seat = nullptr;

    wl_keyboard* keyboard = nullptr;
    xkb_context* keyboardContext = nullptr;
    xkb_keymap* keyboardKeymap = nullptr;
    xkb_state* keyboardState = nullptr;
    bool keyboardActive = true;

    wl_pointer* pointer = nullptr;

    wp_cursor_shape_manager_v1* cursorShapeManager = nullptr;
    wp_cursor_shape_device_v1* cursorShapeDevice = nullptr;

    zwp_pointer_constraints_v1* pointerConstraints = nullptr;
    zwp_locked_pointer_v1* pointerLock = nullptr;

    zwp_relative_pointer_manager_v1* relativePointerManager = nullptr;
    zwp_relative_pointer_v1* relativePointer = nullptr;

    bool pointerActive = true;
    bool pointerLocked = false;
    uint32_t pointerSerial = 0;

    // XDG

    CELL_FUNCTION_INTERNAL static void XDGManagerPing(void* data, xdg_wm_base* manager, const uint32_t serial);

    CELL_FUNCTION_INTERNAL static void XDGSurfaceConfigure(void* data, xdg_surface* surface, const uint32_t serial);

    CELL_FUNCTION_INTERNAL static void XDGToplevelConfigure(void* data, xdg_toplevel* toplevel, const int width, const int height, wl_array* states);
    CELL_FUNCTION_INTERNAL static void XDGToplevelClose(void* data, xdg_toplevel* toplevel);
    CELL_FUNCTION_INTERNAL static void XDGToplevelConfigureBounds(void* data, xdg_toplevel* toplevel, const int32_t width, const int32_t height);
    CELL_FUNCTION_INTERNAL static void XDGToplevelAnnounceWMCapabilities(void* data, xdg_toplevel* toplevel, wl_array* capabilities);

    CELL_FUNCTION_INTERNAL static void XDGDecorationConfigure(void* data, zxdg_toplevel_decoration_v1* decoration, const uint32_t mode);

    static constexpr xdg_wm_base_listener XDGManagerListener = {
        .ping = &Linux::XDGManagerPing
    };

    static constexpr xdg_surface_listener XDGSurfaceListener = {
        .configure = &Linux::XDGSurfaceConfigure,
    };

    static constexpr xdg_toplevel_listener XDGToplevelListener = {
        .configure = &Linux::XDGToplevelConfigure,
        .close = &Linux::XDGToplevelClose,
        .configure_bounds = &Linux::XDGToplevelConfigureBounds,
        .wm_capabilities = &Linux::XDGToplevelAnnounceWMCapabilities
    };

    static constexpr zxdg_toplevel_decoration_v1_listener XDGDecorationListener = {
        .configure = &Linux::XDGDecorationConfigure,
    };

    xdg_wm_base* xdgManager = nullptr;

    xdg_surface* xdgSurface = nullptr;
    xdg_toplevel* xdgToplevel = nullptr;

    zxdg_decoration_manager_v1* xdgDecorationManager = nullptr;
    zxdg_toplevel_decoration_v1* xdgDecoration = nullptr;

    // Extensions

    zwp_idle_inhibit_manager_v1* idleInhibitManager = nullptr;
    zwp_idle_inhibitor_v1* idleInhibitor = nullptr;
};

}
