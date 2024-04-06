// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use crate::ffi::{
    PhysicalDevice, PhysicalDeviceFeaturesEx, VkInstance, VkResult, {PhysicalDeviceMemoryProperties, PhysicalDeviceProperties, QueueFamilyProperties},
};

extern "C" {
    pub fn vkEnumeratePhysicalDevices(instance: VkInstance, count: *mut u32, devices: *mut PhysicalDevice) -> VkResult;
    pub fn vkGetPhysicalDeviceProperties(device: PhysicalDevice, properties: *mut PhysicalDeviceProperties);
    pub fn vkGetPhysicalDeviceFeatures2(device: PhysicalDevice, features: *mut PhysicalDeviceFeaturesEx);
    pub fn vkGetPhysicalDeviceQueueFamilyProperties(device: PhysicalDevice, count: *mut u32, properties: *mut QueueFamilyProperties);
    pub fn vkGetPhysicalDeviceMemoryProperties(device: PhysicalDevice, properties: *mut PhysicalDeviceMemoryProperties);
}
