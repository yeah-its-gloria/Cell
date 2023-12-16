// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use core::ffi::{c_char, c_void};

use crate::ffi::{FenceCreateFlags, Flags, PhysicalDeviceFeatures, StructureType};

#[repr(C)]
pub struct DeviceQueueCreateInfo {
    pub s_type: StructureType,
    pub next: *mut c_void,
    pub flags: u32,
    pub family_index: u32,
    pub count: u32,
    pub priorities: *const f32,
}

#[repr(C)]
pub struct DeviceCreateInfo {
    pub s_type: StructureType,
    pub next: *mut c_void,
    pub flags: u32,
    pub queue_create_info_count: u32,
    pub queue_create_infos: *const DeviceQueueCreateInfo,
    pub layer_count: u32,
    pub layers: *const *const c_char,
    pub extension_count: u32,
    pub extensions: *const *const c_char,
    pub features: *const PhysicalDeviceFeatures,
}

#[repr(C)]
pub struct SemaphoreCreateInfo {
    pub s_type: StructureType,
    pub next: *mut c_void,
    pub flags: Flags,
}

#[repr(C)]
pub struct FenceCreateInfo {
    pub s_type: StructureType,
    pub next: *mut c_void,
    pub flags: FenceCreateFlags,
}
