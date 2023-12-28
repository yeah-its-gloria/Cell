// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use core::ffi::c_char;

use cell_core::interop::Opaque;

#[allow(non_camel_case_types)]
pub type wl_display = *mut Opaque;

#[allow(non_camel_case_types)]
pub type wl_registry = *mut Opaque;

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct wl_message {
    pub name: *const c_char,
    pub signature: *const c_char,
    pub types: *const *const wl_interface,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct wl_interface {
    pub name: *const c_char,
    pub version: i32,
    pub request_count: i32,
    pub requests: *const wl_message,
    pub event_count: i32,
    pub events: *const wl_message,
}

extern "C" {
    pub static wl_registry_interface: wl_interface;

    pub fn wl_display_connect(name: *const c_char) -> wl_display;
    pub fn wl_display_disconnect(display: wl_display);
    pub fn wl_display_roundtrip(display: wl_display) -> i32;

    pub fn wl_proxy_marshal_constructor(proxy: *mut Opaque, opcode: u32, interface: *const wl_interface) -> *mut Opaque;
    pub fn wl_proxy_destroy(proxy: *mut Opaque);
}
