// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

mod structures;
pub use structures::*;

pub type Bool32 = u32;
pub type Flags = u32;
pub type Version = u32;

pub const VK_TRUE: Bool32 = 1;
pub const VK_FALSE: Bool32 = 0;

#[repr(i32)]
pub enum StructureType {
    ApplicationInfo = 0,
    InstanceCreateInfo = 1,
    DeviceQueueCreateInfo = 2,
    DeviceCreateInfo = 3,
    FenceCreateInfo = 8,
    SemaphoreCreateInfo = 9,
    ImageCreateInfo = 14,
    SwapchainCreateInfo = 1000001000,
    Win32SurfaceCreateInfo = 1000009000,
    PhysicalDeviceDynamicRenderingFeatures = 1000044003,
    PhysicalDeviceFeatures = 1000059000, // We only use Features2
    PhysicalDeviceExtendedDynamicStateFeatures = 1000267000,
}

#[repr(i32)]
#[derive(Debug, PartialEq)]
pub enum VkResult {
    Success = 0,
    NotReady = 1,
    Timeout = 2,
    EventSet = 3,
    EventReset = 4,
    Incomplete = 5,
    ErrorOutOfHostMemory = -1,
    ErrorOutOfDeviceMemory = -2,
    ErrorDeviceLost = -4,
    ErrorLayerNotPresent = -6,
    ErrorExtensionNotPresent = -7,
    ErrorIncompatibleDriver = -9,
    ErrorSurfaceLost = -1000000000,
}

pub const fn make_version(variant: u32, major: u32, minor: u32, patch: u32) -> Version {
    (variant << 29) | (major << 22) | (minor << 12) | patch
}

pub const fn get_major_version(version: Version) -> u32 {
    (version >> 22) & 0x7f
}

pub const fn get_minor_version(version: Version) -> u32 {
    (version >> 12) & 0x3ff
}
