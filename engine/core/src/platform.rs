// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use std::fmt::{Display, Formatter, Result};

pub enum PlatformId {
    Windows,
    Linux,
}

#[cfg(windows)]
pub const PLATFORM_ID: PlatformId = PlatformId::Windows;

#[cfg(unix)]
pub const PLATFORM_ID: PlatformId = PlatformId::Linux;

pub struct Platform {
    pub a: u32,
}

impl Display for PlatformId {
    fn fmt(&self, f: &mut Formatter<'_>) -> Result {
        match self {
            Self::Windows => write!(f, "Windows"),
            Self::Linux => write!(f, "Linux"),
        }
    }
}
