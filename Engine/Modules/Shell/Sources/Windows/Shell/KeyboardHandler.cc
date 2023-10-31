// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/Windows.hh>
#include <Cell/System/Log.hh>

namespace Cell::Shell::Implementations {

const size_t KeyLUTOffset = 0x30;

const Shell::KeyboardButton KeyLUT[43] = {// TODO: special keys and such
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
    Shell::KeyboardButton::Z
};

void Windows::HandleKeyInput(WPARAM key, bool isPressed) {
    if (key - KeyLUTOffset > sizeof(KeyLUT) / sizeof(Shell::KeyboardButton) || key < KeyLUTOffset) {
        System::Log("Unimplemented key: %d, %s", key, isPressed ? "pressed" : "released");
        return;
    }

    const Shell::KeyboardButton value = KeyLUT[key - KeyLUTOffset];
    if (value == Shell::KeyboardButton::None) {
        return;
    }
    if (isPressed) {
        this->keys |= value;
    } else {
        this->keys ^= value;
    }
}

}
