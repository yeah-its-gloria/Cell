// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use alloc::{ffi::CString, vec};
use core::{
    ffi::{c_char, c_void},
    mem::transmute,
    ptr::{null, null_mut},
};

use crate::{
    ffi::{
        make_version, vk_create_instance, vk_destroy_instance, vk_get_instance_proc_addr, ApplicationInfo, CommandBeginRenderingFn, CommandEndRenderingFn,
        CommandSetCullModeFn, InstanceCreateInfo, StructureType, VkInstance, VkResult,
    },
    Error,
};

/// Base Vulkan instance.
pub struct Instance {
    pub(crate) instance: VkInstance,
    begin_render: CommandBeginRenderingFn,
    end_render: CommandEndRenderingFn,
    set_cull_mode: CommandSetCullModeFn,
}

fn load_external_func(inst: VkInstance, name: &'static str) -> extern "C" fn() {
    let pointer = unsafe { vk_get_instance_proc_addr(inst, name.as_ptr() as *const c_char) };

    // ...lol
    #[allow(useless_ptr_null_checks)]
    if pointer as *const c_void == null() {
        panic!("vkGetInstanceProcAddr failed for \"{}\"", name);
    }

    pointer
}

#[macro_export]
macro_rules! extension_list {
    ($($x:expr),*) => {{ vec![$($x.as_ptr() as *const core::ffi::c_char,)*] }};
}

impl Instance {
    pub fn new() -> Result<Self, Error> {
        let extensions = extension_list!["VK_KHR_surface\0", if cfg!(windows) { "VK_KHR_win32_surface\0" } else { "VK_KHR_wayland_surface\0" }];

        let name = CString::new("Cell").unwrap();

        let application_info = ApplicationInfo {
            s_type: StructureType::ApplicationInfo,
            next: null_mut(),

            application_name: name.as_ptr(),
            application_version: make_version(0, 1, 0, 0),

            engine_name: name.as_ptr(),
            engine_version: make_version(0, 1, 0, 0),

            api_version: make_version(0, 1, 2, 0),
        };

        let instance_info = InstanceCreateInfo {
            s_type: StructureType::InstanceCreateInfo,
            next: null_mut(),
            flags: 0,

            application_info: &application_info,

            enabled_layer_count: 0,
            enabled_layer_names: null(),

            enabled_extension_count: extensions.len() as u32,
            enabled_extension_names: extensions.as_ptr(),
        };

        let mut inst: VkInstance = null_mut();
        match unsafe { vk_create_instance(&instance_info, null(), &mut inst) } {
            VkResult::Success => {
                Ok(Instance {
                    instance: inst,
                    begin_render: unsafe { transmute(load_external_func(inst, "vkCmdBeginRenderingKHR\0")) },
                    end_render: unsafe { transmute(load_external_func(inst, "vkCmdEndRenderingKHR\0")) },
                    set_cull_mode: unsafe { transmute(load_external_func(inst, "vkCmdSetCullModeEXT\0")) },
                })
            }

            VkResult::ErrorOutOfHostMemory => Err(Error::OutOfHostMemory),
            VkResult::ErrorOutOfDeviceMemory => Err(Error::OutOfDeviceMemory),
            VkResult::ErrorExtensionNotPresent => Err(Error::ExtensionsMissing),
            VkResult::ErrorLayerNotPresent => Err(Error::LayersMissing),
            VkResult::ErrorIncompatibleDriver => Err(Error::IncompatibleDriver),

            _ => {
                panic!("vkCreateInstance failed");
            }
        }
    }
}

impl Drop for Instance {
    fn drop(self: &mut Self) {
        unsafe {
            vk_destroy_instance(self.instance, null());
        }
    }
}
