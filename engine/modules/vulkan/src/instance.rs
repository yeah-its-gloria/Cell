// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use std::{
    ffi::{c_char, CString},
    ptr::{null, null_mut}, mem::transmute,
};
use std::ffi::c_void;

use cell_core::platform::{PlatformId, PLATFORM_ID};

use crate::ffi::{
    constants::VERSION_1_2,
    dynamic::{CommandBeginRenderingFn, CommandEndRenderingFn},
    functions::GetInstanceFunctionAddress,
    types,
};
use crate::ffi::{
    extensions::{
        VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
    },
    functions::{CreateInstance, DestroyInstance},
    result::VkResult,
    structures::{ApplicationInfo, InstanceCreateInfo},
    versioning,
};
use crate::Instance;
use crate::{ffi::enumerations::StructureType, Error};

impl Instance {
    pub fn new() -> Result<Self, Error> {
        let mut extensions = Vec::<*const c_char>::new();

        let surface_ext = CString::new(VK_KHR_SURFACE_EXTENSION_NAME).unwrap();

        let surface_impl_ext = CString::new(match PLATFORM_ID {
            PlatformId::Windows => VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
            PlatformId::Linux => VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME,
        })
        .unwrap();

        extensions.push(surface_ext.as_ptr());
        extensions.push(surface_impl_ext.as_ptr());

        let name = CString::new("Cell").unwrap();

        let application_info = ApplicationInfo {
            s_type: StructureType::ApplicationInfo,
            next: null_mut(),

            application_name: name.as_ptr(),
            application_version: versioning::make_version(0, 1, 0, 0),

            engine_name: name.as_ptr(),
            engine_version: versioning::make_version(0, 1, 0, 0),

            api_version: VERSION_1_2,
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

        let mut inst: types::Instance = null_mut();
        match unsafe { CreateInstance(&instance_info, null(), &mut inst) } {
            VkResult::Success => {}

            _ => {
                return Err(Error::Unknown);
            }
        };

        let begin_render_name = CString::new("vkCmdBeginRenderingKHR").unwrap();
        let begin_render: CommandBeginRenderingFn = unsafe {
            let pointer = GetInstanceFunctionAddress(inst, begin_render_name.as_ptr());
            assert_ne!(pointer as *const c_void, null());

            transmute(pointer)
        };

        let end_render_name = CString::new("vkCmdEndRenderingKHR").unwrap();
        let end_render: CommandEndRenderingFn = unsafe {
            let pointer = GetInstanceFunctionAddress(inst, end_render_name.as_ptr());
            assert_ne!(pointer as *const c_void, null());

            transmute(pointer)
        };

        return Ok(Instance {
            instance: inst,
            begin_render: begin_render,
            end_render: end_render,
        });
    }

    pub fn destroy(self: &mut Self) {
        unsafe {
            DestroyInstance(self.instance, null());
        }
    }
}
