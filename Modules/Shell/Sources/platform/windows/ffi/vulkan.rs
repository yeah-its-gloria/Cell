// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use core::ffi::c_void;

use cell_module_vulkan::surface_details::{AllocationCallbacks, StructureType, SurfaceKHR, VkInstance, VkResult};

use super::{HINSTANCE, HWND};

#[cfg(windows)]
#[repr(C)]
pub struct Win32SurfaceCreateInfo {
    pub s_type: StructureType,
    pub next: *mut c_void,
    pub flags: u32,
    pub instance: HINSTANCE,
    pub window: HWND,
}

#[cfg(windows)]
extern "C" {
    #[link_name = "vkCreateWin32SurfaceKHR"]
    pub fn CreateWin32Surface(instance: VkInstance, info: *const Win32SurfaceCreateInfo, allocator: *const AllocationCallbacks, surface: *mut SurfaceKHR) -> VkResult;
}
