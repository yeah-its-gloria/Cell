// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use alloc::format;

use crate::interop::StringWideEncodeable;

extern "C" {
    #[link_name = "OutputDebugStringW"]
    pub fn OutputDebugString(output: *const u16);
}

/// Internal implementation of the logging function.
pub fn _log(message: &str) {
    let msg = format!("{}\n", message);

    unsafe { OutputDebugString(msg.to_wide().as_ptr()) };
}
