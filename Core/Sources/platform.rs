// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use core::fmt::{Display, Formatter, Result};

/// Represents the identity of a supported platform.
pub enum PlatformId {
    Windows,
    Linux,
}

/// Represents the current platform; Windows.
#[cfg(windows)]
pub const CURRENT_PLATFORM: PlatformId = PlatformId::Windows;

/// Represents the current platform; Linux.
#[cfg(unix)]
pub const CURRENT_PLATFORM: PlatformId = PlatformId::Linux;

impl Display for PlatformId {
    fn fmt(&self, f: &mut Formatter<'_>) -> Result {
        match self {
            Self::Windows => write!(f, "Windows"),
            Self::Linux => write!(f, "Linux"),
        }
    }
}
