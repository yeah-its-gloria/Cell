// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

//! Shell module for Cell; implements shell related functionality, like window/surfaces and input.

#![no_std]

extern crate alloc;

mod keyboard;

#[cfg_attr(windows, path = "platform/windows/mod.rs")]
#[cfg_attr(unix, path = "platform/linux/mod.rs")]
mod os_impl;

#[cfg(windows)]
use os_impl::Windows as OSImplementation;

#[cfg(unix)]
use os_impl::Linux as OSImplementation;

pub use self::keyboard::KeyboardButton;

/// Error values for shell related functionality.
#[derive(Debug, PartialEq)]
pub enum Error {
    InvalidParameters,
}

/// Shell interface.
pub trait Shell: cell_module_vulkan::surface_details::SurfaceProvider {
    /// Creates a new shell session.
    /// This usually involves spawning a window, although some platforms display a mere surface, like consoles or kiosk systems.
    fn init(title: &str, width: u32, height: u32) -> Result<Self, Error> where Self: Sized;

    /// Changes the extent of the current shell surface.
    /// This is in respect to its drawable area; the underlying subsystem is asked to account for its required sizes.
    fn update_extent(&mut self, width: u32, height: u32) -> Result<(), Error>;

    /// Retrieves the drawable area (width, height) of the current shell surface.
    fn retrieve_extent(&self) -> (u32, u32);

    /// Updates the title displayed by the shell.
    fn update_title(&mut self, new: &str) -> Result<(), Error>;

    /// Runs the dispatch loop of the shell.
    /// Returns whether the shell still considers the session active.
    fn run_dispatch(&mut self) -> Result<bool, Error>;

    /// Requests the shell to end the session.
    fn request_quit(&mut self) -> Result<(), Error>;

    /// Returns whether the session is still active.
    fn is_active(&self) -> bool;

    /// Returns whether the current session is in the foreground.
    /// Useful to determine if input should have an impact on state.
    fn is_in_foreground(&self) -> bool;

    /// Retrieves the current status of the keyboard.
    fn retrieve_keys(&self) -> KeyboardButton;
}

/// Creates a shell instance for the current platform.
pub fn create_default_shell(title: &str, width: u32, height: u32) -> Result<impl Shell, Error> {
    OSImplementation::init(title, width, height)
}

#[cfg(not(any(windows, unix)))]
compile_error!("No shell implementation!");
