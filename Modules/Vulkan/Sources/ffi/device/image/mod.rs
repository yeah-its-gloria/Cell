// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use core::ffi::c_void;

use cell_core::interop::Opaque;

use crate::ffi::{AllocationCallbacks, Extent3D, MemoryRequirements, SharingMode, StructureType, VkDevice, VkResult};

mod enumerations;
pub use enumerations::*;

pub type VkImage = *mut Opaque;
pub type VkImageView = *mut Opaque;

#[repr(C)]
pub struct ImageCreateInfo {
    pub s_type: StructureType,
    pub next: *mut c_void,
    pub flags: u32,
    pub image_type: ImageType,
    pub format: Format,
    pub extent: Extent3D,
    pub mip_levels: u32,
    pub array_layers: u32,
    pub samples: SampleCount,
    pub tiling: TilingMode,
    pub usage: ImageUsageFlags,
    pub sharing_mode: SharingMode,
    pub queue_family_index_count: u32,
    pub queue_family_indices: *const u32,
    pub initial_layout: ImageLayout,
}

extern "C" {
    pub fn vkCreateImage(device: VkDevice, info: *const ImageCreateInfo, allocator: *const AllocationCallbacks, image: *mut VkImage) -> VkResult;
    pub fn vkDestroyImage(device: VkDevice, image: VkImage, allocator: *const AllocationCallbacks);
    pub fn vkGetImageMemoryRequirements(device: VkDevice, image: VkImage, requirements: *mut MemoryRequirements);
}
