// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use core::ffi::c_void;

#[repr(C)]
pub struct AllocationCallbacks {
    pub user_data: *const c_void,
    pub fn_allocation: *const c_void,
    pub fn_reallocation: *const c_void,
    pub fn_free: *const c_void,
    pub fn_internal_allocation: *const c_void,
    pub fn_internal_free: *const c_void,
}

#[repr(C)]
#[derive(Clone, Copy)]
pub struct Extent2D {
    pub width: u32,
    pub height: u32,
}

#[repr(C)]
#[derive(Clone)]
pub struct Extent3D {
    pub width: u32,
    pub height: u32,
    pub depth: u32,
}

#[repr(C)]
pub struct Offset2D {
    x: i32,
    y: i32,
}

#[repr(C)]
pub struct Rect2D {
    pub offset: Offset2D,
    pub extent: Extent2D,
}
