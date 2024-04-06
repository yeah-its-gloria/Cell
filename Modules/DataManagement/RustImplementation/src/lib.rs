// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Claus

/// Test struct for Rust object interop purposes.
pub struct Test {
    value: u8,
}

impl Test {
    fn new() -> Self {
        Test { value: 2 }
    }

    fn get_value(&self) -> u8 {
        self.value
    }
}

/// Constructs a new Test object.
#[no_mangle]
pub extern "C" fn TestNew() -> *mut Test {
    Box::into_raw(Box::new(Test::new()))
}

/// Drops the created Test object.
#[no_mangle]
pub unsafe extern "C" fn TestDestruct(t: *mut Test) {
    drop(Box::from_raw(t));
}

/// Retrieves the value stored in this Test object.
#[no_mangle]
pub extern "C" fn TestGetValue(t: *mut Test) -> u8 {
    unsafe { (*t).get_value() }
}
