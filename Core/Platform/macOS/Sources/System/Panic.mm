// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Panic.hh>

#include <stdio.h>
#include <stdlib.h>

#include <AppKit/AppKit.h>

namespace Cell::System {

void SoftPanic() {
    @autoreleasepool {
        NSAlert* alert = [[NSAlert alloc] init];

        [alert setAlertStyle: NSAlertStyleCritical];
        [alert setMessageText: @"A fatal error occurred."];

        [alert runModal];

        [NSApp terminate: nil];
    }

    abort();
}

}
