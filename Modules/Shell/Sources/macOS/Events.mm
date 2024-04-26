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

// Window implementation
@implementation CellWindowImpl
-(BOOL) canBecomeKeyWindow {
    return YES;
}

-(BOOL) canBecomeMainWindow {
    return YES;
}

-(void) keyDown: (NSEvent*) event {
    CELL_ASSERT(event.type == NSEventTypeKeyDown && self.keysRef != nullptr && self.keyLock != nullptr);

    if (event.ARepeat == YES) {
        return;
    }

    if (event.keyCode > sizeof(KeyLUT) || KeyLUT[event.keyCode] == KeyboardButton::None) {
#ifdef _DEBUG
        const char* identifier = [event.characters cStringUsingEncoding: NSUTF8StringEncoding];
        Cell::System::Log("Unknown Key Pressed #%; %", event.keyCode, identifier);
#endif
        return;
    }

    self.keyLock->Lock();
    *self.keysRef |= KeyLUT[event.keyCode];
    self.keyLock->Unlock();
}

-(void) keyUp: (NSEvent*) event {
    CELL_ASSERT(event.type == NSEventTypeKeyUp && self.keysRef != nullptr && self.keyLock != nullptr);

    if (event.ARepeat == YES) {
        return;
    }

    if (event.keyCode > sizeof(KeyLUT) || KeyLUT[event.keyCode] == KeyboardButton::None) {
#ifdef _DEBUG
        const char* identifier = [event.characters cStringUsingEncoding: NSUTF8StringEncoding];
        Cell::System::Log("Unknown Key Released #%; %", event.keyCode, identifier);
#endif
        return;
    }

    self.keyLock->Lock();
    *self.keysRef ^= KeyLUT[event.keyCode];
    self.keyLock->Unlock();
}

-(void) flagsChanged: (NSEvent*) event {
    CELL_ASSERT(event.type == NSEventTypeFlagsChanged && self.keysRef != nullptr && self.keyLock != nullptr);

    // Ignored keys: Caps Lock, Function (fn), Command, Numeric Pad keys, Help
    if ((event.modifierFlags & (NSEventModifierFlagShift | NSEventModifierFlagControl | NSEventModifierFlagOption)) == 0) {
        return;
    }

    self.keyLock->Lock();

    // BUG: sometimes modifiers seem to cause keys to get stuck
    // macOS doesn't seem to care about which side was pressed, therefore we treat them the same

    if (event.modifierFlags & NSEventModifierFlagShift) {
        *self.keysRef &= KeyboardButton::LeftShift;
    } else {
        *self.keysRef ^= KeyboardButton::LeftShift;
    }

    if (event.modifierFlags & NSEventModifierFlagControl) {
        *self.keysRef &= KeyboardButton::LeftControl;
    } else {
        *self.keysRef ^= KeyboardButton::LeftControl;
    }

    if (event.modifierFlags & NSEventModifierFlagOption) {
        *self.keysRef &= KeyboardButton::LeftAlt;
    } else {
        *self.keysRef ^= KeyboardButton::LeftAlt;
    }

    self.keyLock->Unlock();
}
@end

// Delegate implementation
@implementation CellWindowDelegate
-(id) initWithRefToIsDone: (bool*) isDone andIsActivated: (bool*) isActivated {
    CellWindowDelegate* obj = [super init];

    obj.isDone      = isDone;
    obj.isActivated = isActivated;

    return obj;
}

-(BOOL) windowShouldClose: (NSWindow*) sender {
    (void)(sender);

    *self.isDone = true;
    return NO;
}

-(void) windowDidBecomeKey: (NSNotification*) notification {
    (void)(notification);

    *self.isActivated = true;
}

-(void) windowDidResignKey: (NSNotification*) notification {
    (void)(notification);

    *self.isActivated = false;
}
@end
