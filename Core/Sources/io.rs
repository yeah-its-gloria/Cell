// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

pub use crate::os_impl::File as File;

#[derive(Debug)]
pub enum Error {
    NotFound,
    AccessDenied,
    AlreadyExists,
    InvalidOperation,
    InvalidSize,
    NotEnoughMemory,
    ReachedEnd
}

/// Options for accessing files.
#[derive(Clone, Copy, Debug)]
pub struct AccessOptions {
    pub(crate) read: bool,
    pub(crate) write: bool,
    pub(crate) share: bool
}

impl AccessOptions {
    pub fn new() -> Self {
        AccessOptions { read: true, write: false, share: false }
    }

    /// Whether this file should be readable.
    pub fn read(&mut self, b: bool) -> Self {
        self.read = b;

        *self
    }

    /// Whether this file should be writable.
    pub fn write(&mut self, b: bool) -> Self {
        self.write = b;

        *self
    }

    /// Whether this file should be shared with other programs concurrently.
    pub fn share(&mut self, b: bool)-> Self  {
        self.share = b;

        *self
    }
}
