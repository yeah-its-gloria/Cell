// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#[repr(u32)]
#[derive(Debug, PartialEq)]
pub enum WindowMessage {
    Null = 0,
    Create = 1,
    Destroy = 2,
    Activate = 6,
    Close = 16,
    Quit = 18,
    SettingsChanged = 26,
    GetMinMaxSizeInfo = 36,
    KeyDown = 256,
    KeyUp = 257,
    SystemCommand = 274,
}

bitflags::bitflags! {
    #[repr(C)]
    #[derive(Clone, Copy, Debug)]
    pub struct WindowClassStyle: u32 {
        const HorizontalRedraw = 0x0000001;
        const VerticalRedraw = 0x0000002;
        const OwnedDeviceContext = 0x0000020;
    }

    #[repr(C)]
    #[derive(Clone, Copy, Debug)]
    pub struct WindowStyle : u32 {
        const IsVisible = 0x10000000;
        const HasBorder = 0x00800000;
        const HasFullFrame = 0x00400000;
        const HasSystemMenu = 0x00080000;
    }

    #[repr(C)]
    #[derive(Clone, Copy, Debug)]
    pub struct WindowStyleEx : u32 {
        const HasDropShadow = 0x00000100;
    }
}

#[repr(u32)]
#[derive(Debug, PartialEq)]
pub enum PeekMessageOptions {
    NoRemove = 0,
    Remove = 1,
    NoYield = 2,
}

#[repr(i32)]
#[derive(Debug)]
pub enum WindowPointerIndex {
    RegularUserData = -21,
}

#[repr(u32)]
#[derive(Debug)]
pub enum DWMWindowAttribute {
    UseImmersiveDarkMode = 20,
}

#[derive(Debug, PartialEq)]
pub enum WindowActivationState {
    Inactivated,
    Activated,
    ClickActivated,
}

impl From<u64> for WindowActivationState {
    fn from(a: u64) -> Self {
        match a {
            0 => Self::Inactivated,
            1 => Self::Activated,
            2 => Self::ClickActivated,

            unknown => {
                panic!("Invalid value {} for WindowActivationState", unknown);
            }
        }
    }
}
