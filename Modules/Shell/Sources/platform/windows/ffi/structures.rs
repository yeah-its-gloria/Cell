// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use core::ptr::null_mut;

use cell_core::interop::Opaque;

use super::{WindowClassStyle, WindowMessage, HINSTANCE, HWND};

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

#[repr(C)]
pub struct MinMaxSizeInfo {
    pub reserved_x: i32,
    pub reserved_y: i32,
    pub max_size_x: i32,
    pub max_size_y: i32,
    pub max_position_x: i32,
    pub max_position_y: i32,
    pub min_track_size_x: i32,
    pub min_track_size_y: i32,
    pub max_track_size_x: i32,
    pub max_track_size_y: i32,
}

#[repr(C)]
pub struct Rectangle {
    pub left: i32,
    pub top: i32,
    pub right: i32,
    pub bottom: i32,
}

#[repr(C)]
pub struct WindowClassEx {
    pub size: u32,
    pub style: WindowClassStyle,
    pub window_procedure: extern "C" fn(HWND, WindowMessage, u64, i64) -> i64,
    pub class_extra: i32,
    pub window_extra: i32,
    pub instance: HINSTANCE,
    pub icon: *mut Opaque,
    pub cursor: *mut Opaque,
    pub brush: *mut Opaque,
    pub menu_name: *const u16,
    pub class_name: *const u16,
    pub icon_small: *mut Opaque,
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

impl Default for MinMaxSizeInfo {
    fn default() -> Self {
        Self {
            reserved_x: 0,
            reserved_y: 0,
            max_size_x: 0,
            max_size_y: 0,
            max_position_x: 0,
            max_position_y: 0,
            min_track_size_x: 0,
            min_track_size_y: 0,
            max_track_size_x: 0,
            max_track_size_y: 0,
        }
    }
}

impl Default for Rectangle {
    fn default() -> Self {
        Self {
            left: 0,
            top: 0,
            right: 0,
            bottom: 0,
        }
    }
}
