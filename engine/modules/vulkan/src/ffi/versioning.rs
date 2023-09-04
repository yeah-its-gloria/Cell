// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

pub type Version = u32;

pub const fn make_version(variant: u32, major: u32, minor: u32, patch: u32) -> Version {
    (variant << 29) | (major << 22) | (minor << 12) | patch
}

pub const fn get_major_version(version: Version) -> u32 {
    (version >> 22) & 0x7f
}

pub const fn get_minor_version(version: Version) -> u32 {
    (version >> 12) & 0x3ff
}
