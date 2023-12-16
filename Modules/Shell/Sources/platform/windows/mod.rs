// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use alloc::boxed::Box;
use core::ptr::{null, null_mut};

use cell_core::interop::{Opaque, StringWideEncodeable};
use cell_module_vulkan::surface_details::{StructureType, SurfaceKHR, SurfaceProvider, VkInstance, VkResult};

use crate::{
    os_impl::ffi::{
        vulkan::{CreateWin32Surface, Win32SurfaceCreateInfo},
        AdjustWindowRectangle, GetClientRectangle, GetSystemDPI, PostMessage, Rectangle, SetWindowPosition, SetWindowText, WindowMessage, WindowStyle, WindowStyleEx,
        FALSE, HINSTANCE, HWND, TRUE,
    },
    Error, KeyboardButton, Shell,
};

mod ffi;

mod dispatch;
mod key_input;
mod new_drop;

pub struct SharedData {
    in_foreground: bool,
    keys: KeyboardButton,
}

pub struct Windows {
    instance: HINSTANCE,
    window: HWND,
    brush: *mut Opaque,
    active: bool,
    shared: Box<SharedData>,
}

pub fn extent_calculate(width: u32, height: u32, style: WindowStyle, style_ex: WindowStyleEx) -> Rectangle {
    let mut rectangle = Rectangle {
        left: 32,
        top: 32,
        right: 32 + width as i32,
        bottom: 32 + height as i32,
    };

    let result = unsafe { AdjustWindowRectangle(&mut rectangle, style, FALSE, style_ex, GetSystemDPI()) };
    assert_eq!(result, TRUE);

    rectangle
}

impl Shell for Windows {
    fn init(title: &str, width: u32, height: u32) -> Result<Self, Error> {
        Self::new(title, width, height)
    }

    fn run_dispatch(&mut self) -> Result<bool, Error> {
        self.run_dispatch_impl()
    }

    fn request_quit(&mut self) -> Result<(), Error> {
        unsafe { PostMessage(self.window, WindowMessage::Close, 0, 0) };

        Ok(())
    }

    fn is_active(&self) -> bool {
        self.active
    }

    fn is_in_foreground(&self) -> bool {
        (*self.shared).in_foreground
    }

    fn update_title(&mut self, new: &str) -> Result<(), Error> {
        let result = unsafe { SetWindowText(self.window, new.to_wide().as_ptr()) };
        assert_eq!(result, TRUE);

        Ok(())
    }

    fn retrieve_keys(&self) -> KeyboardButton {
        (*self.shared).keys
    }

    fn update_extent(self: &mut Self, width: u32, height: u32) -> Result<(), Error> {
        let rectangle = extent_calculate(
            width,
            height,
            WindowStyle::HasBorder | WindowStyle::HasFullFrame | WindowStyle::HasSystemMenu,
            WindowStyleEx::HasDropShadow,
        );

        let result = unsafe { SetWindowPosition(self.window, null_mut(), rectangle.left, rectangle.top, rectangle.right, rectangle.bottom, 0x2) };
        assert_eq!(result, TRUE);

        Ok(())
    }

    fn retrieve_extent(self: &Self) -> (u32, u32) {
        let mut rectangle = Rectangle::default();

        let result = unsafe { GetClientRectangle(self.window, &mut rectangle) };
        assert_eq!(result, TRUE);

        ((rectangle.right - rectangle.left) as u32, (rectangle.bottom - rectangle.top) as u32)
    }
}

impl SurfaceProvider for Windows {
    fn create_surface(&self, instance: VkInstance) -> Result<SurfaceKHR, cell_module_vulkan::Error> {
        let info = Win32SurfaceCreateInfo {
            s_type: StructureType::Win32SurfaceCreateInfo,
            next: null_mut(),
            flags: 0,
            instance: self.instance,
            window: self.window,
        };

        let mut surface: SurfaceKHR = null_mut();
        match unsafe { CreateWin32Surface(instance, &info, null(), &mut surface) } {
            VkResult::Success => Ok(surface),

            VkResult::ErrorOutOfHostMemory => Err(cell_module_vulkan::Error::OutOfHostMemory),
            VkResult::ErrorOutOfDeviceMemory => Err(cell_module_vulkan::Error::OutOfDeviceMemory),

            _ => {
                panic!("CreateWin32Surface failed");
            }
        }
    }

    fn get_extent(&self) -> (u32, u32) {
        self.retrieve_extent()
    }
}
