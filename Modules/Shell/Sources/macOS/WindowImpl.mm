// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/macOS.hh>

#include <Cell/System/Log.hh>

using namespace Cell::Shell;

// TODO: implement more keys

const KeyboardButton KeyLUT[52] = {
    KeyboardButton::A,
    KeyboardButton::S,
    KeyboardButton::D,
    KeyboardButton::F,
    KeyboardButton::H,
    KeyboardButton::G,
    KeyboardButton::Z,
    KeyboardButton::X,
    KeyboardButton::C,
    KeyboardButton::V,
    KeyboardButton::None,
    KeyboardButton::B,
    KeyboardButton::Q,
    KeyboardButton::W,
    KeyboardButton::E,
    KeyboardButton::R,
    KeyboardButton::Y,
    KeyboardButton::T,
    KeyboardButton::One,
    KeyboardButton::Two,
    KeyboardButton::Three,
    KeyboardButton::Four,
    KeyboardButton::Five,
    KeyboardButton::Six,
    KeyboardButton::None,
    KeyboardButton::Nine,
    KeyboardButton::Seven,
    KeyboardButton::None,
    KeyboardButton::Eight,
    KeyboardButton::Zero,
    KeyboardButton::None,
    KeyboardButton::O,
    KeyboardButton::U,
    KeyboardButton::None,
    KeyboardButton::I,
    KeyboardButton::P,
    KeyboardButton::Return,
    KeyboardButton::L,
    KeyboardButton::J,
    KeyboardButton::None,
    KeyboardButton::K,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::None,
    KeyboardButton::N,
    KeyboardButton::M,
    KeyboardButton::None,
    KeyboardButton::Tab,
    KeyboardButton::Space,
    KeyboardButton::None,
    KeyboardButton::Backspace
};

@implementation CellWindowImpl
-(BOOL) canBecomeKeyWindow {
    return YES;
}

-(BOOL) canBecomeMainWindow {
    return YES;
}

-(void) keyDown: (NSEvent*) event {
    CELL_ASSERT(event.type == NSEventTypeKeyDown && self.keysRef != nullptr);

    if (event.keyCode > sizeof(KeyLUT) || KeyLUT[event.keyCode] == KeyboardButton::None) {
#ifdef _DEBUG
        const char* identifier = [event.characters cStringUsingEncoding: NSUTF8StringEncoding];
        Cell::System::Log("Unknown Key Pressed #%; %", event.keyCode, identifier);
#endif
        return;
    }

    *self.keysRef |= KeyLUT[event.keyCode];
}

-(void) keyUp: (NSEvent*) event {
    CELL_ASSERT(event.type == NSEventTypeKeyUp && self.keysRef != nullptr);

    if (event.keyCode > sizeof(KeyLUT) || KeyLUT[event.keyCode] == KeyboardButton::None) {
#ifdef _DEBUG
        const char* identifier = [event.characters cStringUsingEncoding: NSUTF8StringEncoding];
        Cell::System::Log("Unknown Key Released #%; %", event.keyCode, identifier);
#endif
        return;
    }

    *self.keysRef ^= KeyLUT[event.keyCode];
}
@end
