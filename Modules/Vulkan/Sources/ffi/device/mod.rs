// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use cell_core::interop::Opaque;

use super::{AllocationCallbacks, Flags, PhysicalDevice, VkResult};

mod structs;
pub use structs::*;

pub type VkDevice = *mut Opaque;
pub type Fence = *mut Opaque;
pub type Semaphore = *mut Opaque;
pub type Queue = *mut Opaque;

bitflags::bitflags! {
    #[repr(C)]
    #[derive(Clone, Debug)]
    pub struct FenceCreateFlags : Flags {
        const Signaled = 0x1;
    }
}

extern "C" {
    #[link_name = "vkCreateDevice"]
    pub fn CreateDevice(base: PhysicalDevice, info: *const DeviceCreateInfo, allocator: *const AllocationCallbacks, device: *mut VkDevice) -> VkResult;

    #[link_name = "vkDestroyDevice"]
    pub fn DestroyDevice(device: VkDevice, allocator: *const AllocationCallbacks);

    #[link_name = "vkGetDeviceQueue"]
    pub fn GetDeviceQueue(device: VkDevice, family: u32, index: u32, queue: &mut Queue);

    #[link_name = "vkDeviceWaitIdle"]
    pub fn WaitForDeviceIdle(device: VkDevice) -> VkResult;

    #[link_name = "vkCreateFence"]
    pub fn CreateFence(device: VkDevice, info: *const FenceCreateInfo, allocator: *const AllocationCallbacks, fence: *mut Fence) -> VkResult;

    #[link_name = "vkDestroyFence"]
    pub fn DestroyFence(device: VkDevice, fence: Fence, allocator: *const AllocationCallbacks);

    #[link_name = "vkCreateSemaphore"]
    pub fn CreateSemaphore(device: VkDevice, info: *const SemaphoreCreateInfo, allocator: *const AllocationCallbacks, semaphore: *mut Semaphore) -> VkResult;

    #[link_name = "vkDestroySemaphore"]
    pub fn DestroySemaphore(device: VkDevice, semaphore: Semaphore, allocator: *const AllocationCallbacks);
}
