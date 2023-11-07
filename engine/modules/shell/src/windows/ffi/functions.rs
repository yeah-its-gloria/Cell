// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use std::{ffi::c_void, ptr::null_mut};

use cell_core::interop::Opaque;

use super::{structures::{Message, WindowClassEx}, types::{HWND, BOOL, HINSTANCE, WindowMessage, PeekMessageOptions}};

extern "C" {
    #[link_name = "GetModuleHandleW"]
    pub fn GetModuleHandle(module_name: *const u16) -> HINSTANCE;

    #[link_name = "RegisterClassExW"]
    pub fn RegisterClass(class: *const WindowClassEx) -> u16;

    #[link_name = "UnregisterClassW"]
    pub fn UnregisterClass(class_name: *const u16, instance: *const Opaque) -> BOOL;

    #[link_name = "CreateWindowExW"]
    pub fn CreateWindow(
        ex_style: u32,
        class_name: *const u16,
        window_name: *const u16,
        style: u32,
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
    pub fn SetTitleForWindow(window: HWND, string: *const u16) -> BOOL;

    #[link_name = "DefWindowProcW"]
    pub fn DefaultWindowProcedure(window: HWND, message: WindowMessage, high: u64, low: i64) -> i64;

    pub fn PostQuitMessage(exit_code: i32);

    #[link_name = "PeekMessageW"]
    pub fn PeekMessage(
        message: *mut Message,
        window: HWND,
        filter_min: u32,
        filter_max: u32,
        options: PeekMessageOptions,
    ) -> BOOL;

    pub fn TranslateMessage(message: *mut Message) -> BOOL;

    #[link_name = "DispatchMessageW"]
    pub fn DispatchMessage(message: *mut Message) -> i64;

    pub fn DwmSetWindowAttribute(window: HWND, id: u32, data: *const c_void, size: u32) -> i32;

    pub fn CreateSolidBrush(color: u32) -> *mut Opaque;

    pub fn DeleteObject(object: *mut Opaque) -> BOOL;
}

impl Default for Message {
    fn default() -> Self {
        Self {
            window: null_mut(),
            message: WindowMessage::Null,
            high: 0,
            low: 0,
            time: 0,
            point_x: 0,
            point_y: 0,
        }
    }
}
