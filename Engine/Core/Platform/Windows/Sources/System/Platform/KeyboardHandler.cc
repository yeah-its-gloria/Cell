// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Log.hh>
#include <Cell/System/Platform/Windows.hh>

namespace Cell::System::Platform {

const size_t KeyLUTOffset = 0x30;

const Shell::KeyValue KeyLUT[43] = // TODO: special keys and such
{
    Shell::KeyValue::Zero,
    Shell::KeyValue::One,
    Shell::KeyValue::Two,
    Shell::KeyValue::Three,
    Shell::KeyValue::Four,
    Shell::KeyValue::Five,
    Shell::KeyValue::Six,
    Shell::KeyValue::Seven,
    Shell::KeyValue::Eight,
    Shell::KeyValue::Nine,
    Shell::KeyValue::None,
    Shell::KeyValue::None,
    Shell::KeyValue::None,
    Shell::KeyValue::None,
    Shell::KeyValue::None,
    Shell::KeyValue::None,
    Shell::KeyValue::None,
    Shell::KeyValue::A,
    Shell::KeyValue::B,
    Shell::KeyValue::C,
    Shell::KeyValue::D,
    Shell::KeyValue::E,
    Shell::KeyValue::F,
    Shell::KeyValue::G,
    Shell::KeyValue::H,
    Shell::KeyValue::I,
    Shell::KeyValue::J,
    Shell::KeyValue::K,
    Shell::KeyValue::L,
    Shell::KeyValue::M,
    Shell::KeyValue::N,
    Shell::KeyValue::O,
    Shell::KeyValue::P,
    Shell::KeyValue::Q,
    Shell::KeyValue::R,
    Shell::KeyValue::S,
    Shell::KeyValue::T,
    Shell::KeyValue::U,
    Shell::KeyValue::V,
    Shell::KeyValue::W,
    Shell::KeyValue::X,
    Shell::KeyValue::Y,
    Shell::KeyValue::Z
};

void Windows::HandleKeyInput(WPARAM key, bool isPressed) {
    if (key - KeyLUTOffset > sizeof(KeyLUT) / sizeof(Shell::KeyValue) || key < KeyLUTOffset) {
        System::Log("Unimplemented key: %d, %s", key, isPressed ? "pressed" : "released");
        return;
    }

    const Shell::KeyValue value = KeyLUT[key - KeyLUTOffset];
    if (value == Shell::KeyValue::None) {
        return;
    }

    this->keyLock->Lock();

    if (isPressed) {
        this->keys |= value;
    } else {
        this->keys ^= value;
    }

    this->keyLock->Unlock();
}

}
