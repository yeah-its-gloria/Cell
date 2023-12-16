// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use super::{ffi::MapVirtualKey, KeyboardButton};

const KEY_LUT: [KeyboardButton; 164] = [
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::Backspace,
    KeyboardButton::Tab,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::Return,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::Escape,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::Space,
    KeyboardButton::None, // PageUp
    KeyboardButton::None, // PageDown
    KeyboardButton::None, // End
    KeyboardButton::None, // Home
    KeyboardButton::LeftArrow,
    KeyboardButton::UpArrow,
    KeyboardButton::RightArrow,
    KeyboardButton::DownArrow,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None, // Insert
    KeyboardButton::None, // Delete
    KeyboardButton::None,
    KeyboardButton::Zero,
    KeyboardButton::One,
    KeyboardButton::Two,
    KeyboardButton::Three,
    KeyboardButton::Four,
    KeyboardButton::Five,
    KeyboardButton::Six,
    KeyboardButton::Seven,
    KeyboardButton::Eight,
    KeyboardButton::Nine,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::A,
    KeyboardButton::B,
    KeyboardButton::C,
    KeyboardButton::D,
    KeyboardButton::E,
    KeyboardButton::F,
    KeyboardButton::G,
    KeyboardButton::H,
    KeyboardButton::I,
    KeyboardButton::J,
    KeyboardButton::K,
    KeyboardButton::L,
    KeyboardButton::M,
    KeyboardButton::N,
    KeyboardButton::O,
    KeyboardButton::P,
    KeyboardButton::Q,
    KeyboardButton::R,
    KeyboardButton::S,
    KeyboardButton::T,
    KeyboardButton::U,
    KeyboardButton::V,
    KeyboardButton::W,
    KeyboardButton::X,
    KeyboardButton::Y,
    KeyboardButton::Z,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None, // NumPadZero
    KeyboardButton::None, // NumPadOn
    KeyboardButton::None, // NumPadTwo
    KeyboardButton::None, // NumPadThree
    KeyboardButton::None, // NumPadFour
    KeyboardButton::None, // NumPadFive
    KeyboardButton::None, // NumPadSix
    KeyboardButton::None, // NumPadEight
    KeyboardButton::None, // NumPadNine
    KeyboardButton::None, // NumPadMultiply
    KeyboardButton::None, // NumPadAdd
    KeyboardButton::None, // NumPadSeparator (?)
    KeyboardButton::None, // NumPadSubtract
    KeyboardButton::None, // NumPadDecimal
    KeyboardButton::None, // NumPadDivide
    KeyboardButton::F1,
    KeyboardButton::F2,
    KeyboardButton::F3,
    KeyboardButton::F4,
    KeyboardButton::F5,
    KeyboardButton::F6,
    KeyboardButton::F7,
    KeyboardButton::F8,
    KeyboardButton::F9,
    KeyboardButton::F10,
    KeyboardButton::F11,
    KeyboardButton::F12,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::LeftShift,
    KeyboardButton::RightShift,
    KeyboardButton::LeftControl,
    KeyboardButton::RightControl,
    KeyboardButton::LeftAlt,
    KeyboardButton::RightAlt,
    // Rest of the keys go unused.
];

impl KeyboardButton {
    pub fn update(&mut self, key: u64, extra_info: i64, pressed: bool) {
        let is_right = (extra_info >> 16) & 0x0100 != 0; // KF_EXTENDED
        let processed_key = match key {
            // VK_SHIFT
            0x10 => {
                unsafe {
                    MapVirtualKey(((extra_info >> 16) & 0xff) as u32, 3) as u64 // MAPVK_VSC_TO_VK_EX
                }
            }

            // VK_CONTROL
            0x11 => {
                if is_right {
                    0xa3 // VK_RCONTROL
                } else {
                    0xa2 // VK_LCONTROL
                }
            }

            // VK_MENU
            0x12 => {
                if is_right {
                    0xa5 // VK_RMENU
                } else {
                    0xa4 // VK_LMENU
                }
            }

            // VK_LWIN, VK_RWIN; we do not want to process these keys
            0x5b | 0x5c => u64::MAX,

            a => a,
        };

        if processed_key > KEY_LUT.len() as u64 {
            return;
        }

        let value = KEY_LUT[(processed_key - 1) as usize];
        if pressed {
            *self |= value;
        } else {
            *self ^= value;
        }
    }
}
