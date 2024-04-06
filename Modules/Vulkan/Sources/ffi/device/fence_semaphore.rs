// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use core::ffi::c_void;

use cell_core::interop::Opaque;

use crate::ffi::{AllocationCallbacks, Flags, StructureType, VkResult};

use super::VkDevice;

pub type Fence = *mut Opaque;
pub type Semaphore = *mut Opaque;

#[repr(C)]
pub struct FenceCreateInfo {
    pub s_type: StructureType,
    pub next: *mut c_void,
    pub flags: FenceCreateFlags,
}

bitflags::bitflags! {
    #[repr(C)]
    #[derive(Clone, Debug)]
    pub struct FenceCreateFlags : Flags {
        const Signaled = 0x1;
    }
}

#[repr(C)]
pub struct SemaphoreCreateInfo {
    pub s_type: StructureType,
    pub next: *mut c_void,
    pub flags: Flags,
}

extern "C" {
    #[link_name = "vkCreateFence"]
    pub fn CreateFence(device: VkDevice, info: *const FenceCreateInfo, allocator: *const AllocationCallbacks, fence: *mut Fence) -> VkResult;

    #[link_name = "vkDestroyFence"]
    pub fn DestroyFence(device: VkDevice, fence: Fence, allocator: *const AllocationCallbacks);

    #[link_name = "vkCreateSemaphore"]
    pub fn CreateSemaphore(device: VkDevice, info: *const SemaphoreCreateInfo, allocator: *const AllocationCallbacks, semaphore: *mut Semaphore) -> VkResult;

    #[link_name = "vkDestroySemaphore"]
    pub fn DestroySemaphore(device: VkDevice, semaphore: Semaphore, allocator: *const AllocationCallbacks);
}
