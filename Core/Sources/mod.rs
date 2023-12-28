// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

//! Core Cell crate, providing very commonly used functionality for the engine.

// TODO: custom allocator and panic handler
#![no_std]

extern crate alloc;

pub mod interop;

mod platform;
mod logging;

pub use platform::*;
pub use logging::*;

#[cfg_attr(windows, path = "platform/windows/mod.rs")]
#[cfg_attr(unix, path = "platform/linux/mod.rs")]
mod os_impl;

#[cfg(not(any(windows, unix)))]
compile_error!("No core implementation!");
