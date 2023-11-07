// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use cell_core::interop::Opaque;

use super::types::{HWND, HINSTANCE, WindowMessage};

#[repr(C)]
pub struct WindowClassEx {
    pub size: u32,
    pub style: u32,
    pub window_proc: extern "C" fn(HWND, WindowMessage, u64, i64) -> i64,
    pub class_extra: i32,
    pub window_extra: i32,
    pub instance: HINSTANCE,
    pub icon: *mut Opaque,
    pub cursor: *mut Opaque,
    pub background_brush: *mut Opaque,
    pub menu_name: *const u16,
    pub class_name: *const u16,
    pub icon_small: *mut Opaque,
}

#[repr(C)]
pub struct Message {
    pub window: HWND,
    pub message: WindowMessage,
    pub high: u64,
    pub low: i64,
    pub time: u32,
    pub point_x: i32,
    pub point_y: i32,
}
