// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#[repr(i32)]
pub enum StructureType {
    ApplicationInfo = 0,
    InstanceCreateInfo = 1,
    PhysicalDeviceDynamicRenderingFeatures = 1000044003,
    PhysicalDeviceFeatures = 1000059000, // We only use Features2
}

#[repr(u32)]
#[allow(dead_code)]
pub enum PhysicalDeviceType {
    Other = 0,
    IntegratedGPU = 1,
    DiscreteGPU = 2,
    VirtualGPU = 3,
    CPU = 4,
}

#[repr(u32)]
#[allow(dead_code)]
pub enum SampleCountFlag {
    None = 0,
    OneBit = 0x01,
    TwoBit = 0x02,
    FourBit = 0x04,
    EightBit = 0x08,
    SixteenBit = 0x10,
    ThirtyTwoBit = 0x20,
    SixtyFourBit = 0x40,
}

#[repr(u32)]
#[allow(dead_code)]
pub enum RenderingFlag {
    ContentsSecondaryCommandBuffersBit = 0x00000001,
    SuspendingBit = 0x00000002,
    ResumingBit = 0x00000004,
    EnableLegacyDitheringBitExt = 0x00000008,
}

#[repr(u32)]
#[allow(dead_code)]
pub enum ImageLayout {
    Undefined = 0,
    General = 1,
    ColorAttachmentOptimal = 2,
    DepthStencilAttachmentOptimal = 3,
    DepthStencilReadOnlyOptimal = 4,
    ShaderReadOnlyOptimal = 5,
    TransferSrcOptimal = 6,
    TransferDstOptimal = 7,
    Preinitialised = 8,
    DepthReadOnlyStencilAttachmentOptimal = 1000117000,
    DepthAttachmentStencilReadOnlyOptimal = 1000117001,
    DepthAttachmentOptimal = 1000241000,
    DepthReadOnlyOptimal = 1000241001,
    StencilAttachmentOptimal = 1000241002,
    StencilReadOnlyOptimal = 1000241003,
    ReadOnlyOptimal = 1000314000,
    AttachmentOptimal = 1000314001,
    PresentSrcKhr = 1000001002,
}

#[repr(u32)]
#[allow(dead_code)]
pub enum AttachmentLoadOp {
    Load = 0,
    Clear = 1,
    DoNotCare = 2,
}

#[repr(u32)]
#[allow(dead_code)]
pub enum AttachmentStoreOp {
    Store = 0,
    DoNotCare = 1,
    None = 1000301000,
}

#[repr(u32)]
#[allow(dead_code)]
pub enum ResolveModeFlags {
    None = 0,
    SampleZeroBit = 0x00000001,
    AverageBit = 0x00000002,
    MinBit = 0x00000004,
    MaxBit = 0x00000008,
}
