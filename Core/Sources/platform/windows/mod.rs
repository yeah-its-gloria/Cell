// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

// cfg test stuff is purely to make rust-analyzer work (this language server sucks)

use alloc::format;
use core::{ffi::c_void, ptr::null_mut};

#[cfg(not(test))]
use core::panic::PanicInfo;

#[cfg(not(test))]
use super::log;

mod allocator;
mod file;

use crate::interop::StringWideEncodeable;
use allocator::CellAllocator;
pub use file::*;

extern "C" {
    fn IsDebuggerPresent() -> i32;
    fn OutputDebugStringW(output: *const u16);
    fn GetStdHandle(which: u32) -> *mut c_void;
    fn WriteConsoleW(out: *mut c_void, buffer: *const c_void, count: u32, out_count: *mut u32, reserved: *mut c_void) -> i32;
    fn GetCurrentProcess() -> *mut c_void;
    fn TerminateProcess(process: *mut c_void, code: u32) -> i32;
}

#[global_allocator]
static GLOBAL: CellAllocator = CellAllocator;

#[cfg(not(test))]
#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    log!("{}", info);

    unsafe { TerminateProcess(GetCurrentProcess(), 1) };
    loop {}
}

trait HandleExtensions {
    fn is_invalid_handle(&self) -> bool;
}

impl HandleExtensions for *mut c_void {
    fn is_invalid_handle(&self) -> bool {
        *self as usize == usize::MAX || self.is_null()
    }
}

/// Internal implementation of the logging function.
pub fn _log(message: &str) {
    let msg = format!("{}\n", message);

    let mut count: u32 = 0;
    unsafe {
        if IsDebuggerPresent() == 1 {
            OutputDebugStringW(msg.to_wide().as_ptr());
            return;
        }

        WriteConsoleW(
            GetStdHandle(-11 as i32 as u32),
            msg.to_wide().as_ptr() as *const c_void,
            msg.len() as u32,
            &mut count,
            null_mut(),
        );
    };
}
