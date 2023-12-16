// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use core::ffi::{c_char, c_void};

use cell_core::interop::Opaque;

use super::{AllocationCallbacks, Flags, StructureType, Version, VkResult};

pub type VkInstance = *mut Opaque;

#[repr(C)]
pub struct ApplicationInfo {
    pub s_type: StructureType,
    pub next: *mut c_void,
    pub application_name: *const c_char,
    pub application_version: Version,
    pub engine_name: *const c_char,
    pub engine_version: Version,
    pub api_version: Version,
}

#[repr(C)]
pub struct InstanceCreateInfo {
    pub s_type: StructureType,
    pub next: *mut c_void,
    pub flags: Flags,
    pub application_info: *const ApplicationInfo,
    pub enabled_layer_count: u32,
    pub enabled_layer_names: *const *const c_char,
    pub enabled_extension_count: u32,
    pub enabled_extension_names: *const *const c_char,
}

extern "C" {
    #[link_name = "vkCreateInstance"]
    pub fn vk_create_instance(info: *const InstanceCreateInfo, allocator: *const AllocationCallbacks, instance: *mut VkInstance) -> VkResult;

    #[link_name = "vkDestroyInstance"]
    pub fn vk_destroy_instance(instance: VkInstance, allocator: *const AllocationCallbacks);

    #[link_name = "vkGetInstanceProcAddr"]
    pub fn vk_get_instance_proc_addr(instance: VkInstance, name: *const c_char) -> extern "C" fn();
}
