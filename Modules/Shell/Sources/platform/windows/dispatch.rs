// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use core::{ffi::c_void, mem::size_of};

use super::{
    ffi::{
        DWMWindowAttribute, DefaultWindowProcedure, DispatchMessage, DwmSetWindowAttribute, GetWindowPointer, Message, MinMaxSizeInfo, PeekMessage, PeekMessageOptions,
        PostQuitMessage, TranslateMessage,
        WindowActivationState::{self, Activated, ClickActivated},
        WindowMessage::{self, Activate, Close, Create, GetMinMaxSizeInfo, KeyDown, KeyUp, Quit, SettingsChanged, SystemCommand},
        WindowPointerIndex, BOOL, HWND, TRUE,
    },
    Error, SharedData, Windows,
};

pub extern "C" fn window_procedure(window: HWND, message: WindowMessage, high: u64, low: i64) -> i64 {
    let shared_ptr: *mut SharedData = unsafe { GetWindowPointer(window, WindowPointerIndex::RegularUserData) as *mut SharedData };

    match message {
        Create | SettingsChanged => {
            // TODO: check registry whether we're supposed to be in immersive mode

            let value = TRUE;
            let result = unsafe {
                DwmSetWindowAttribute(
                    window,
                    DWMWindowAttribute::UseImmersiveDarkMode,
                    &value as *const u32 as *const c_void,
                    size_of::<BOOL>() as u32,
                )
            };

            assert_eq!(result, 0);

            return 0;
        }

        Activate => {
            if !shared_ptr.is_null() {
                unsafe {
                    (*shared_ptr).in_foreground = match WindowActivationState::from(high) {
                        Activated | ClickActivated => true,
                        _ => false,
                    }
                };
            }

            return 0;
        }

        Close => {
            unsafe { PostQuitMessage(0) };
            return 0;
        }

        GetMinMaxSizeInfo => {
            let info = low as *mut MinMaxSizeInfo;
            if !info.is_null() {
                unsafe {
                    (*info).min_track_size_x = 1280;
                    (*info).min_track_size_y = 720;

                    (*info).max_track_size_x = 4096;
                    (*info).max_track_size_y = 2160;
                }
            }

            return 0;
        }

        SystemCommand => {
            // GET_SC_WPARAM
            match high & 0xfff0 {
                // SC_MONITORPOWER
                0xf170 => {
                    if low != -1 {
                        return 0;
                    }
                }

                // SC_SCREENSAVE
                0xf140 => {
                    return 0;
                }

                _ => {}
            }
        }

        KeyDown | KeyUp => {
            if !shared_ptr.is_null() {
                unsafe {
                    (*shared_ptr).keys.update(high, low, message == KeyDown);
                }
            }
        }

        _ => {}
    };

    return unsafe { DefaultWindowProcedure(window, message, high, low) };
}

impl Windows {
    pub fn run_dispatch_impl(&mut self) -> Result<bool, Error> {
        let mut has_processed = false;

        let mut message = Message::default();
        while unsafe { PeekMessage(&mut message, self.window, 0, 0, PeekMessageOptions::Remove) } == TRUE {
            has_processed = true;

            unsafe {
                TranslateMessage(&mut message);
                DispatchMessage(&mut message);
            }
        }

        if has_processed {
            if message.message == Quit {
                self.active = false;
            }
        }

        Ok(self.active)
    }
}
