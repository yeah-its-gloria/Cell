// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/Windows.hh>
#include <Cell/System/Log.hh>

namespace Cell::Shell::Implementations {

const Shell::KeyboardButton KeyLUT[165] = {
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::Backspace,
    Shell::KeyboardButton::Tab,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::Return,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::Escape,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::Space,
    Shell::KeyboardButton::None, // PageUp
    Shell::KeyboardButton::None, // PageDown
    Shell::KeyboardButton::None, // End
    Shell::KeyboardButton::None, // Home
    Shell::KeyboardButton::LeftArrow,
    Shell::KeyboardButton::UpArrow,
    Shell::KeyboardButton::RightArrow,
    Shell::KeyboardButton::DownArrow,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None, // Insert
    Shell::KeyboardButton::None, // Delete
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::Zero,
    Shell::KeyboardButton::One,
    Shell::KeyboardButton::Two,
    Shell::KeyboardButton::Three,
    Shell::KeyboardButton::Four,
    Shell::KeyboardButton::Five,
    Shell::KeyboardButton::Six,
    Shell::KeyboardButton::Seven,
    Shell::KeyboardButton::Eight,
    Shell::KeyboardButton::Nine,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::A,
    Shell::KeyboardButton::B,
    Shell::KeyboardButton::C,
    Shell::KeyboardButton::D,
    Shell::KeyboardButton::E,
    Shell::KeyboardButton::F,
    Shell::KeyboardButton::G,
    Shell::KeyboardButton::H,
    Shell::KeyboardButton::I,
    Shell::KeyboardButton::J,
    Shell::KeyboardButton::K,
    Shell::KeyboardButton::L,
    Shell::KeyboardButton::M,
    Shell::KeyboardButton::N,
    Shell::KeyboardButton::O,
    Shell::KeyboardButton::P,
    Shell::KeyboardButton::Q,
    Shell::KeyboardButton::R,
    Shell::KeyboardButton::S,
    Shell::KeyboardButton::T,
    Shell::KeyboardButton::U,
    Shell::KeyboardButton::V,
    Shell::KeyboardButton::W,
    Shell::KeyboardButton::X,
    Shell::KeyboardButton::Y,
    Shell::KeyboardButton::Z,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None, // NumPadZero
    Shell::KeyboardButton::None, // NumPadOn
    Shell::KeyboardButton::None, // NumPadTwo
    Shell::KeyboardButton::None, // NumPadThree
    Shell::KeyboardButton::None, // NumPadFour
    Shell::KeyboardButton::None, // NumPadFive
    Shell::KeyboardButton::None, // NumPadSix
    Shell::KeyboardButton::None, // NumPadEight
    Shell::KeyboardButton::None, // NumPadNine
    Shell::KeyboardButton::None, // NumPadMultiply
    Shell::KeyboardButton::None, // NumPadAdd
    Shell::KeyboardButton::None, // NumPadSeparator (?)
    Shell::KeyboardButton::None, // NumPadSubtract
    Shell::KeyboardButton::None, // NumPadDecimal
    Shell::KeyboardButton::None, // NumPadDivide
    Shell::KeyboardButton::F1,
    Shell::KeyboardButton::F2,
    Shell::KeyboardButton::F3,
    Shell::KeyboardButton::F4,
    Shell::KeyboardButton::F5,
    Shell::KeyboardButton::F6,
    Shell::KeyboardButton::F7,
    Shell::KeyboardButton::F8,
    Shell::KeyboardButton::F9,
    Shell::KeyboardButton::F10,
    Shell::KeyboardButton::F11,
    Shell::KeyboardButton::F12,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::None,
    Shell::KeyboardButton::LeftShift,
    Shell::KeyboardButton::RightShift,
    Shell::KeyboardButton::LeftControl,
    Shell::KeyboardButton::RightControl,
    Shell::KeyboardButton::LeftAlt,
    Shell::KeyboardButton::RightAlt

    // Rest of the keys go unused.
};

void Windows::HandleKeyInput(const WPARAM key, const LPARAM extInfo, const bool isPressed) {
    WPARAM newKey = key;

    const bool isRight = (extInfo >> 16) & KF_EXTENDED;
    switch (key) {
    case VK_SHIFT: {
        newKey = MapVirtualKeyW((extInfo >> 16) & 0xff, MAPVK_VSC_TO_VK_EX);
        break;
    }

    case VK_CONTROL: {
        newKey = isRight ? VK_RCONTROL : VK_LCONTROL;
        break;
    }

    case VK_MENU: {
        newKey = isRight ? VK_RMENU : VK_LMENU;
        break;
    }

    default: {
        break;
    }
    }

#if _DEBUG
    if (newKey > sizeof(KeyLUT) / sizeof(Shell::KeyboardButton)) {
        System::Log("Unimplemented key: %, %", key, isPressed ? "pressed" : "released");
        return;
    }
#endif

    const Shell::KeyboardButton value = KeyLUT[newKey - 1];
    if (isPressed) {
        this->keys |= value;
    } else {
        this->keys ^= value;
    }
}

}
