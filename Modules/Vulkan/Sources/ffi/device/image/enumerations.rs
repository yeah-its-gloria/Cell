// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use crate::ffi::Flags;

#[repr(u32)]
#[derive(Clone, Copy, PartialEq)]
pub enum Format {
    Undefined = 0,

    #[allow(non_camel_case_types)]
    R8G8B8A8_sRGB = 43,
}

#[repr(u32)]
pub enum ImageType {
    Image1D = 0,
    Image2D = 1,
    Image3D = 2,
}

#[repr(u32)]
pub enum ImageLayout {
    Undefined = 0,
    General = 1,
    ColorAttachmentOptimal = 2,
    DepthStencilAttachmentOptimal = 3,
    DepthStencilReadOnlyOptimal = 4,
    ShaderReadOnlyOptimal = 5,
    TransferSourceOptimal = 6,
    TransferDestinationOptimal = 7,
    Preinitialised = 8,
    DepthReadOnlyStencilAttachmentOptimal = 1000117000,
    DepthAttachmentStencilReadOnlyOptimal = 1000117001,
    DepthAttachmentOptimal = 1000241000,
    DepthReadOnlyOptimal = 1000241001,
    StencilAttachmentOptimal = 1000241002,
    StencilReadOnlyOptimal = 1000241003,
    ReadOnlyOptimal = 1000314000,
    AttachmentOptimal = 1000314001,
    PresentSourceKHR = 1000001002,
}

bitflags::bitflags! {
    #[repr(C)]
    #[derive(Clone, Debug)]
    pub struct ImageUsageFlags : Flags {
        const None = 0;

        const AsTransferSource = 1 << 0;
        const AsTransferDestination = 1 << 1;

        const IsSampled = 1 << 2;
        const IsStorage = 1 << 3;

        const HasColorAttachment = 1 << 4;
        const HasDepthStencilAttachment = 1 << 5;
    }
}

#[repr(u32)]
pub enum SampleCount {
    None = 0,
    OneBit = 1,
    TwoBit = 2,
    FourBit = 4,
    EightBit = 8,
    SixteenBit = 16,
    ThirtyTwoBit = 32,
    SixtyFourBit = 64,
}

#[repr(u32)]
pub enum TilingMode {
    Optimal = 0,
    Linear = 1,
}
