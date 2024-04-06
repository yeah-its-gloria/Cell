// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

//! Core Cell crate, providing very commonly used functionality for the engine.

#![no_std]
pub extern crate alloc;

pub mod io;
pub mod interop;

mod logging;

#[cfg_attr(windows, path = "platform/windows/mod.rs")]
mod os_impl;

pub use logging::*;

#[cfg(not(any(windows)))]
compile_error!("No core implementation!");
