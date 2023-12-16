// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use core::ffi::c_void;

use cell_core::interop::Opaque;

use super::{
    DWMWindowAttribute, Message, PeekMessageOptions, Rectangle, WindowClassEx, WindowMessage, WindowPointerIndex, WindowStyle, WindowStyleEx, BOOL, HCURSOR, HICON,
    HINSTANCE, HWND,
};

extern "C" {
    // Base Win32

    #[link_name = "GetModuleHandleW"]
    pub fn GetModuleHandle(module_name: *const u16) -> HINSTANCE;

    // Window classes

    #[link_name = "RegisterClassExW"]
    pub fn RegisterClass(class: *const WindowClassEx) -> u16;

    #[link_name = "UnregisterClassW"]
    pub fn UnregisterClass(class_name: *const u16, instance: *const Opaque) -> BOOL;

    // Window

    #[link_name = "AdjustWindowRectExForDpi"]
    pub fn AdjustWindowRectangle(rect: *mut Rectangle, style: WindowStyle, has_menu: BOOL, style_ex: WindowStyleEx, dpi: u32) -> BOOL;

    #[link_name = "GetClientRect"]
    pub fn GetClientRectangle(window: HWND, rectangle: *mut Rectangle) -> BOOL;

    #[link_name = "CreateWindowExW"]
    pub fn CreateWindow(
        style_ex: WindowStyleEx,
        class_name: *const u16,
        window_name: *const u16,
        style: WindowStyle,
        x: i32,
        y: i32,
        width: i32,
        height: i32,
        parent: HWND,
        menu: *const Opaque,
        instance: HINSTANCE,
        creation_parameter: *mut c_void,
    ) -> HWND;

    pub fn DestroyWindow(window: HWND) -> BOOL;

    #[link_name = "SetWindowTextW"]
    pub fn SetWindowText(window: HWND, string: *const u16) -> BOOL;

    #[link_name = "GetWindowLongPtrW"]
    pub fn GetWindowPointer(window: HWND, index: WindowPointerIndex) -> *mut c_void;

    #[link_name = "SetWindowLongPtrW"]
    pub fn SetWindowPointer(window: HWND, index: WindowPointerIndex, address: *mut c_void) -> *mut c_void;

    #[link_name = "SetWindowPos"]
    pub fn SetWindowPosition(window: HWND, window_inserted_after: HWND, x: i32, y: i32, width: i32, height: i32, flags: u32) -> BOOL;

    // Resources

    #[link_name = "LoadIconW"]
    pub fn LoadIcon(instance: HINSTANCE, name: *const u16) -> HICON;

    #[link_name = "LoadCursorW"]
    pub fn LoadCursor(instance: HINSTANCE, name: *const u16) -> HCURSOR;

    // Messages & Handling

    #[link_name = "DefWindowProcW"]
    pub fn DefaultWindowProcedure(window: HWND, message: WindowMessage, high: u64, low: i64) -> i64;

    #[link_name = "PostMessageW"]
    pub fn PostMessage(window: HWND, message: WindowMessage, high: u64, low: i64) -> BOOL;

    pub fn PostQuitMessage(exit_code: i32);

    #[link_name = "PeekMessageW"]
    pub fn PeekMessage(message: *mut Message, window: HWND, filter_min: u32, filter_max: u32, options: PeekMessageOptions) -> BOOL;

    pub fn TranslateMessage(message: *mut Message) -> BOOL;

    #[link_name = "DispatchMessageW"]
    pub fn DispatchMessage(message: *mut Message) -> i64;

    // DWM

    pub fn DwmSetWindowAttribute(window: HWND, id: DWMWindowAttribute, data: *const c_void, size: u32) -> i32;

    // GDI

    pub fn CreateSolidBrush(color: u32) -> *mut Opaque;

    pub fn DeleteObject(object: *mut Opaque) -> BOOL;

    // Other

    #[link_name = "GetDpiForSystem"]
    pub fn GetSystemDPI() -> u32;

    #[link_name = "MapVirtualKeyW"]
    pub fn MapVirtualKey(code: u32, map_type: u32) -> u32;
}
