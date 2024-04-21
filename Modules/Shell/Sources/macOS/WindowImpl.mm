// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/macOS.hh>

#include <Cell/System/Log.hh>

// TODO: key LUT

@implementation CellWindowImpl
-(BOOL) canBecomeKeyWindow {
    return YES;
}

-(BOOL) canBecomeMainWindow {
    return YES;
}

-(void) keyDown: (NSEvent*) event {
    CELL_ASSERT(event.type == NSEventTypeKeyDown && self.keysRef != nullptr);

    if (event.keyCode != 3) {
        return;
    }

    *self.keysRef |= Cell::Shell::KeyboardButton::F;
}

-(void) keyUp: (NSEvent*) event {
    CELL_ASSERT(event.type == NSEventTypeKeyUp && self.keysRef != nullptr);

    if (event.keyCode != 3) {
        return;
    }

    *self.keysRef ^= Cell::Shell::KeyboardButton::F;
}
@end
