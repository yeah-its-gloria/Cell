// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use alloc::boxed::Box;
use core::{
    ffi::c_void,
    mem::size_of,
    ptr::{addr_of_mut, null, null_mut},
};

use cell_core::interop::StringWideEncodeable;

use crate::os_impl::ffi::{LoadCursor, LoadIcon};

use super::{
    dispatch::window_procedure,
    extent_calculate,
    ffi::{
        CreateSolidBrush, CreateWindow, DeleteObject, DestroyWindow, GetModuleHandle, RegisterClass, SetWindowPointer, UnregisterClass, WindowClassEx, WindowClassStyle,
        WindowPointerIndex, WindowStyle, WindowStyleEx,
    },
    Error, KeyboardButton, SharedData, Windows,
};

const CLASS_NAME: [u16; 5] = ['C' as u16, 'e' as u16, 'l' as u16, 'l' as u16, 0];

impl Windows {
    pub fn new(title: &str, width: u32, height: u32) -> Result<Self, Error> {
        let instance = unsafe { GetModuleHandle(null()) };
        assert_ne!(instance, null_mut());

        let brush = unsafe { CreateSolidBrush(0) };
        assert_ne!(brush, null_mut());

        let icon = unsafe { LoadIcon(instance, 100 as *const u16) };
        assert_ne!(icon, null_mut());

        let cursor = unsafe { LoadCursor(null_mut(), 32512 as *const u16) }; // IDC_ARROW
        assert_ne!(cursor, null_mut());

        let window_class = WindowClassEx {
            size: size_of::<WindowClassEx>() as u32,
            style: WindowClassStyle::HorizontalRedraw | WindowClassStyle::VerticalRedraw | WindowClassStyle::OwnedDeviceContext,
            window_procedure,
            class_extra: 0,
            window_extra: 0,
            instance,
            icon,
            cursor,
            brush,
            menu_name: null(),
            class_name: CLASS_NAME.as_ptr(),
            icon_small: icon,
        };

        let atom = unsafe { RegisterClass(&window_class) };
        assert_ne!(atom, 0);

        let base_style = WindowStyle::HasBorder | WindowStyle::HasFullFrame | WindowStyle::HasSystemMenu;
        let style_ex = WindowStyleEx::HasDropShadow;

        let rectangle = extent_calculate(width, height, base_style, style_ex);

        let window = unsafe {
            CreateWindow(
                style_ex,
                CLASS_NAME.as_ptr(),
                title.to_wide().as_ptr(),
                WindowStyle::IsVisible | base_style,
                rectangle.left,
                rectangle.top,
                rectangle.right,
                rectangle.bottom,
                null_mut(),
                null(),
                instance,
                null_mut(),
            )
        };

        assert!(!window.is_null());

        let mut shared_box = Box::new(SharedData {
            in_foreground: true,
            keys: KeyboardButton::None,
        });

        unsafe { SetWindowPointer(window, WindowPointerIndex::RegularUserData, addr_of_mut!(*shared_box) as *mut c_void) };

        Ok(Windows {
            instance,
            window,
            brush,
            active: true,
            shared: shared_box,
        })
    }
}

impl Drop for Windows {
    fn drop(&mut self) {
        unsafe {
            DestroyWindow(self.window);
            DeleteObject(self.brush);
            UnregisterClass(CLASS_NAME.as_ptr(), self.instance);
        }
    }
}
