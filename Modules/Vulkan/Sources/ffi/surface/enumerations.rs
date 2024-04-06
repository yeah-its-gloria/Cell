// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#[repr(u32)]
#[derive(Clone, Copy, PartialEq)]
pub enum ColorSpace {
    #[allow(non_camel_case_types)]
    sRGB_Nonlinear = 0,
}

#[repr(u32)]
#[derive(Clone, Copy, PartialEq)]
pub enum PresentMode {
    Immediate = 0,
    Mailbox = 1,
    FIFO = 2,

    #[allow(non_camel_case_types)]
    FIFO_Relaxed = 3,
}

#[repr(u32)]
pub enum CompositeAlphaFlag {
    Opaque = 1,
    PreMultiplied = 2,
    PostMultiplied = 4,
    Inherit = 8,
}

#[repr(u32)]
#[derive(Clone, Copy, Debug)]
pub enum SurfaceTransformFlag {
    Identity = 1 << 0,
    Rotated90 = 1 << 1,
    Rotated180 = 1 << 2,
    Rotated270 = 1 << 3,
}
