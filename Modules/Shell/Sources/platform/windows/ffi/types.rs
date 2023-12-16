// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use cell_core::interop::Opaque;

pub type HINSTANCE = *mut Opaque;
pub type HWND = *mut Opaque;
pub type HICON = *mut Opaque;
pub type HCURSOR = *mut Opaque;

pub type BOOL = u32;

pub const TRUE: BOOL = 1;
pub const FALSE: BOOL = 0;
