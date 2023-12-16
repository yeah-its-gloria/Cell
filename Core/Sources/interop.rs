// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use alloc::{vec::Vec, string::String};
use core::marker::{PhantomData, PhantomPinned};

/// Opaque handle block.
/// Allows usage of handles in the C world that rely on void* or struct pointers.
#[repr(C)]
pub struct Opaque {
    _d: [u8; 0],
    _m: PhantomData<(*mut u8, PhantomPinned)>,
}

/// Trait to allow string like types to implement wide encoding support.
/// Mostly useful on Windows.
pub trait StringWideEncodeable {
    /// Converts the current string to a null terminated wide string, encoded in UTF-16.
    fn to_wide(&self) -> Vec<u16>;
}

impl StringWideEncodeable for String {
    fn to_wide(&self) -> Vec<u16> {
        let mut string16: Vec<u16> = self.encode_utf16().collect();
        string16.push(0);

        string16
    }
}

impl StringWideEncodeable for &str {
    fn to_wide(&self) -> Vec<u16> {
        let mut string16: Vec<u16> = self.encode_utf16().collect();
        string16.push(0);

        string16
    }
}
