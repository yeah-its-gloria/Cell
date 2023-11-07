// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use cell_core::platform::{PlatformId, PLATFORM_ID};
use implementation::Windows;

#[cfg_attr(windows, path = "windows/mod.rs")]
mod implementation;

#[derive(Debug, PartialEq)]
pub enum Error {
    RequestedQuit,
}

// Shell interface.
pub trait Shell {
    fn new(title: &str) -> Result<Self, Error>
    where
        Self: Sized;
    fn change_title(&self, new: &str) -> Result<(), Error>;
    fn run_dispatch(&self) -> Result<(), Error>;
}

// Creates a shell instance for the current platform.
pub fn create_shell(title: &str) -> Result<impl Shell, Error> {
    match PLATFORM_ID {
        PlatformId::Windows => Windows::new(title),

        _ => {
            debug_assert!(false); loop { }
        }
    }
}
