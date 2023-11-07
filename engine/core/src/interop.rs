// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use std::marker::{PhantomData, PhantomPinned};

#[repr(C)]
pub struct Opaque {
    _d: [u8; 0],
    _m: PhantomData<(*mut u8, PhantomPinned)>,
}
