// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

pub use crate::os_impl::_log;

/// Logs a message to the debugger output, preferred by the platform.
#[macro_export]
macro_rules! log {
    ($($arg:tt)*) => {{
        $crate::_log(core::format_args!($($arg)*).as_str().unwrap());
    }};
}
