// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use std::{
    ffi::c_void,
    mem::size_of,
    ptr::{null, null_mut},
};

use cell_core::interop::Opaque;

use crate::{
    implementation::ffi::{
        constants::DWMWA_USE_IMMERSIVE_DARK_MODE,
        functions::{
            CreateSolidBrush, CreateWindow, DefaultWindowProcedure, DeleteObject, DestroyWindow,
            DispatchMessage, DwmSetWindowAttribute, GetModuleHandle, PeekMessage, PostQuitMessage,
            RegisterClass, SetTitleForWindow, TranslateMessage, UnregisterClass,
        },
        structures::{Message, WindowClassEx},
        types::{PeekMessageOptions, WindowMessage, BOOL, HINSTANCE, HWND, TRUE},
    },
    Error, Shell,
};

mod ffi;

const CLASS_NAME: [u16; 5] = ['C' as u16, 'e' as u16, 'l' as u16, 'l' as u16, 0];

pub struct Windows {
    instance: HINSTANCE,
    window: HWND,
    brush: *mut Opaque,
}

extern "C" fn window_proc(window: HWND, message: WindowMessage, high: u64, low: i64) -> i64 {
    match message {
        WindowMessage::Create => {
            let value = TRUE;
            let result = unsafe {
                DwmSetWindowAttribute(
                    window,
                    DWMWA_USE_IMMERSIVE_DARK_MODE,
                    &value as *const u32 as *const c_void,
                    size_of::<BOOL>() as u32,
                )
            };

            assert_eq!(result, 0);

            0
        }

        WindowMessage::Close => {
            unsafe { PostQuitMessage(0) };
            0
        }

        _ => unsafe { DefaultWindowProcedure(window, message, high, low) },
    }
}

fn str_to_utf16(string: &str) -> Vec<u16> {
    let mut string16: Vec<u16> = string.encode_utf16().collect();
    string16.push(0);

    return string16;
}

impl Shell for Windows {
    fn new(title: &str) -> Result<Self, Error> {
        let instance = unsafe { GetModuleHandle(null()) };
        assert_ne!(instance, null_mut());

        let brush = unsafe { CreateSolidBrush(0) };
        assert_ne!(brush, null_mut());

        let window_class = WindowClassEx {
            size: size_of::<WindowClassEx>() as u32,
            style: 0x0001 | 0x0002 | 0x0020,
            window_proc: window_proc,
            class_extra: 0,
            window_extra: 0,
            instance: instance,
            icon: null_mut(),
            cursor: null_mut(),
            background_brush: brush,
            menu_name: null(),
            class_name: CLASS_NAME.as_ptr(),
            icon_small: null_mut(),
        };

        let atom = unsafe { RegisterClass(&window_class) };
        assert_ne!(atom, 0);

        let window = unsafe {
            CreateWindow(
                0x100,
                CLASS_NAME.as_ptr(),
                str_to_utf16(title).as_ptr(),
                0x10c80000,
                16,
                16,
                1280 + 16,
                720 + 16,
                null_mut(),
                null(),
                instance,
                null_mut(),
            )
        };

        assert!(!window.is_null());

        Ok(Windows {
            instance: instance,
            window: window,
            brush: brush,
        })
    }

    fn change_title(&self, new: &str) -> Result<(), Error> {
        unsafe { SetTitleForWindow(self.window, str_to_utf16(new).as_ptr()) };

        Ok(())
    }

    fn run_dispatch(&self) -> Result<(), Error> {
        let mut has_processed = false;

        let mut message = Message::default();
        while unsafe { PeekMessage(&mut message, self.window, 0, 0, PeekMessageOptions::Remove) }
            == TRUE
        {
            has_processed = true;

            unsafe {
                TranslateMessage(&mut message);
                DispatchMessage(&mut message);
            }
        }

        if !has_processed {
            return Ok(());
        }

        if message.message == WindowMessage::Quit {
            return Err(Error::RequestedQuit);
        }

        Ok(())
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
