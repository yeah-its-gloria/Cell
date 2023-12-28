// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use core::ptr::{null, null_mut};

use cell_module_vulkan::surface_details::{SurfaceKHR, SurfaceProvider, VkInstance};

use crate::{Error, KeyboardButton, Shell};

mod ffi;

pub struct Linux {
    display: ffi::wl_display,
    registry: ffi::wl_registry,
}

impl Shell for Linux {
    fn init(_title: &str, _width: u32, _height: u32) -> Result<Self, Error> {
        let display = unsafe { ffi::wl_display_connect(null()) };
        assert_ne!(display, null_mut());

        // opcode is WL_DISPLAY_GET_REGISTRY
        let registry = unsafe { ffi::wl_proxy_marshal_constructor(display, 1, &ffi::wl_registry_interface) };
        assert_ne!(display, null_mut());

        // TODO: implement listeners, surfaces, etc

        let result = unsafe { ffi::wl_display_roundtrip(display) };
        assert_ne!(result, -1);

        Ok(Linux { display, registry })
    }

    fn update_extent(&mut self, _width: u32, _height: u32) -> Result<(), Error> {
        Ok(())
    }

    fn retrieve_extent(&self) -> (u32, u32) {
        (0, 0)
    }

    fn update_title(&mut self, _new: &str) -> Result<(), Error> {
        Ok(())
    }

    fn run_dispatch(&mut self) -> Result<bool, Error> {
        Ok(true)
    }

    fn request_quit(&mut self) -> Result<(), Error> {
        Ok(())
    }

    fn is_active(&self) -> bool {
        true
    }

    fn is_in_foreground(&self) -> bool {
        true
    }

    fn retrieve_keys(&self) -> KeyboardButton {
        KeyboardButton::None
    }
}

impl Drop for Linux {
    fn drop(&mut self) {
        unsafe {
            ffi::wl_proxy_destroy(self.registry);
            ffi::wl_display_disconnect(self.display);
        }
    }
}

impl SurfaceProvider for Linux {
    fn create_surface(&self, _instance: VkInstance) -> Result<SurfaceKHR, cell_module_vulkan::Error> {
        Ok(null_mut())
    }

    fn get_extent(&self) -> (u32, u32) {
        self.retrieve_extent()
    }
}
