// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use super::{AllocationCallbacks, PhysicalDevice, VkResult};

mod fence_semaphore;
mod image;
mod memory;
mod rendering;
mod swapchain;
mod types;

pub use fence_semaphore::*;
pub use image::*;
pub use memory::*;
pub use rendering::*;
pub use swapchain::*;
pub use types::*;

extern "C" {
    #[link_name = "vkCreateDevice"]
    pub fn CreateDevice(base: PhysicalDevice, info: *const DeviceCreateInfo, allocator: *const AllocationCallbacks, device: *mut VkDevice) -> VkResult;

    #[link_name = "vkDestroyDevice"]
    pub fn DestroyDevice(device: VkDevice, allocator: *const AllocationCallbacks);

    #[link_name = "vkGetDeviceQueue"]
    pub fn GetDeviceQueue(device: VkDevice, family: u32, index: u32, queue: &mut Queue);

    #[link_name = "vkDeviceWaitIdle"]
    pub fn WaitForDeviceIdle(device: VkDevice) -> VkResult;
}
