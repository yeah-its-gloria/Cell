// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use std::ffi::c_char;

use super::{result::VkResult, structures::*, types::*};

extern "C" {
    #[link_name = "vkCreateInstance"]
    pub fn CreateInstance(
        create_info: *const InstanceCreateInfo,
        allocator: *const AllocationCallbacks,
        instance: *mut Instance,
    ) -> VkResult;

    #[link_name = "vkDestroyInstance"]
    pub fn DestroyInstance(instance: Instance, allocator: *const AllocationCallbacks);

    #[link_name = "vkEnumeratePhysicalDevices"]
    pub fn EnumeratePhysicalDevices(
        instance: Instance,
        count: *mut u32,
        devices: *mut PhysicalDevice,
    ) -> VkResult;

    #[link_name = "vkGetPhysicalDeviceProperties"]
    pub fn GetPhysicalDeviceProperties(
        device: PhysicalDevice,
        properties: *mut PhysicalDeviceProperties,
    );

    #[link_name = "vkGetPhysicalDeviceFeatures2"]
    pub fn GetPhysicalDeviceFeatures(device: PhysicalDevice, features: *mut PhysicalDeviceFeatures);

    #[link_name = "vkGetInstanceProcAddr"]
    pub fn GetInstanceFunctionAddress(instance: Instance, name: *const c_char) -> extern "C" fn();
}
