// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use cell_core::interop::Opaque;

pub type HINSTANCE = *mut Opaque;
pub type HWND = *mut Opaque;

pub type BOOL = u32;

pub const TRUE: BOOL = 1;
pub const FALSE: BOOL = 0;

#[repr(u32)]
#[derive(Debug, PartialEq)]
pub enum WindowMessage {
    Null = 0,
    Create = 1,
    Destroy = 2,
    Activate = 6,
    Close = 16,
    Quit = 18
}

#[repr(u32)]
#[derive(Debug, PartialEq)]
pub enum PeekMessageOptions {
    NoRemove = 0,
    Remove = 1,
    NoYield = 2
}
