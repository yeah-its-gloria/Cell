// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use super::Flags;

#[repr(i32)]
pub enum StructureType {
    ApplicationInfo = 0,
    InstanceCreateInfo = 1,
    DeviceQueueCreateInfo = 2,
    DeviceCreateInfo = 3,
    FenceCreateInfo = 8,
    SemaphoreCreateInfo = 9,
    SwapchainCreateInfo = 1000001000,
    Win32SurfaceCreateInfo = 1000009000,
    PhysicalDeviceDynamicRenderingFeatures = 1000044003,
    PhysicalDeviceFeatures = 1000059000, // We only use Features2
    PhysicalDeviceExtendedDynamicStateFeatures = 1000267000,
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
pub enum RenderingFlag {
    ContentsSecondaryCommandBuffers = 0x00000001,
    Suspending = 0x00000002,
    Resuming = 0x00000004,
    EnableLegacyDitheringEXT = 0x00000008,
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

        const TransferSource = 1 << 0;
        const TransferDestination = 1 << 1;

        const IsSampled = 1 << 2;
        const IsStorage = 1 << 3;

        const HasColorAttachment = 1 << 4;
        const HasDepthStencilAttachment = 1 << 5;
    }
}

#[repr(u32)]
pub enum AttachmentLoadOp {
    Load = 0,
    Clear = 1,
    DoNotCare = 2,
}

#[repr(u32)]
pub enum AttachmentStoreOp {
    Store = 0,
    DoNotCare = 1,
    None = 1000301000,
}

#[repr(u32)]
pub enum ResolveModeFlag {
    None = 0,
    SampleZero = 1,
    Average = 2,
    Min = 4,
    Max = 8,
}

#[repr(u32)]
#[derive(Clone, Copy, PartialEq)]
pub enum Format {
    Undefined = 0,

    #[allow(non_camel_case_types)]
    R8G8B8A8_sRGB = 43,
}

#[repr(u32)]
pub enum SharingMode {
    Exclusive = 0,
    Concurrent = 1,
}
