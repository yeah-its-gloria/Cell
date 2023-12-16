// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use core::ffi::c_void;

use cell_core::interop::Opaque;

use super::{
    AllocationCallbacks, Bool32, ColorSpace, CompositeAlphaFlag, Extent2D, Flags, Format, PresentMode, SharingMode, StructureType, SurfaceKHR, SurfaceTransformFlag,
    VkDevice, VkResult, Image,
};

pub type SwapchainKHR = *mut Opaque;

#[repr(C)]
pub struct SwapchainCreateInfo {
    pub s_type: StructureType,
    pub next: *mut c_void,
    pub flags: Flags,
    pub surface: SurfaceKHR,
    pub min_image_count: u32,
    pub format: Format,
    pub color_space: ColorSpace,
    pub extent: Extent2D,
    pub array_layers: u32,
    pub usage: Flags,
    pub sharing_mode: SharingMode,
    pub queue_family_index_count: u32,
    pub queue_family_indices: *const u32,
    pub pre_transform: SurfaceTransformFlag,
    pub composite_alpha: CompositeAlphaFlag,
    pub present_mode: PresentMode,
    pub clipped: Bool32,
    pub old_swapchain: SwapchainKHR,
}

extern "C" {
    pub fn vkCreateSwapchainKHR(device: VkDevice, info: *const SwapchainCreateInfo, allocator: *const AllocationCallbacks, swapchain: *mut SwapchainKHR) -> VkResult;
    pub fn vkDestroySwapchainKHR(device: VkDevice, swapchain: SwapchainKHR, allocator: *const AllocationCallbacks);
    pub fn vkGetSwapchainImagesKHR(device: VkDevice, swapchain: SwapchainKHR, count: *mut u32, images: *mut Image) -> VkResult;
}
