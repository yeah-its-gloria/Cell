// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use cell_core::interop::Opaque;

use super::{
    Format, ImageUsageFlags, PhysicalDevice, VkResult, {AllocationCallbacks, Extent2D}, {Bool32, VkInstance},
};

pub type SurfaceKHR = *mut Opaque;

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

#[repr(C)]
pub struct SurfaceCapabilities {
    pub min_image_count: u32,
    pub max_image_count: u32,
    pub current_extent: Extent2D,
    pub min_image_extent: Extent2D,
    pub max_image_extent: Extent2D,
    pub max_image_array_layers: u32,
    pub supported_transforms: SurfaceTransformFlag,
    pub current_transform: SurfaceTransformFlag,
    pub supported_composite_alpha: CompositeAlphaFlag,
    pub supported_usage_flags: ImageUsageFlags,
}

#[repr(C)]
#[derive(Clone)]
pub struct SurfaceFormat {
    pub format: Format,
    pub color_space: ColorSpace,
}

extern "C" {
    #[link_name = "vkDestroySurfaceKHR"]
    pub fn DestroySurface(instance: VkInstance, surface: SurfaceKHR, allocator: *const AllocationCallbacks);

    #[link_name = "vkGetPhysicalDeviceSurfaceSupportKHR"]
    pub fn GetSurfaceSupport(device: PhysicalDevice, queue_family_index: u32, surface: SurfaceKHR, supported: *mut Bool32) -> VkResult;

    #[link_name = "vkGetPhysicalDeviceSurfaceCapabilitiesKHR"]
    pub fn GetSurfaceCapabilities(device: PhysicalDevice, surface: SurfaceKHR, capabilities: *mut SurfaceCapabilities) -> VkResult;

    #[link_name = "vkGetPhysicalDeviceSurfaceFormatsKHR"]
    pub fn GetSurfaceFormats(device: PhysicalDevice, surface: SurfaceKHR, count: *mut u32, formats: *mut SurfaceFormat) -> VkResult;

    #[link_name = "vkGetPhysicalDeviceSurfacePresentModesKHR"]
    pub fn GetSurfacePresentModes(device: PhysicalDevice, surface: SurfaceKHR, count: *mut u32, modes: *mut PresentMode) -> VkResult;
}

impl Default for SurfaceCapabilities {
    fn default() -> Self {
        SurfaceCapabilities {
            min_image_count: 0,
            max_image_count: 0,
            current_extent: Extent2D { width: 0, height: 0 },
            min_image_extent: Extent2D { width: 0, height: 0 },
            max_image_extent: Extent2D { width: 0, height: 0 },
            max_image_array_layers: 0,
            supported_transforms: SurfaceTransformFlag::Identity,
            current_transform: SurfaceTransformFlag::Identity,
            supported_composite_alpha: CompositeAlphaFlag::Opaque,
            supported_usage_flags: ImageUsageFlags::None,
        }
    }
}
