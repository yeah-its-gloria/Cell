// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Panic.hh>

#include <AppKit/AppKit.h>
#include <dispatch/dispatch.h>

namespace Cell::System {

void SoftPanic() {
    dispatch_sync(dispatch_get_main_queue(), ^{
        NSAlert* alert = [NSAlert new];

        [alert setAlertStyle: NSAlertStyleCritical];
        [alert setMessageText: @"A fatal error occurred."];

        [alert runModal];

        [NSApp terminate: nullptr];
    });

    abort();
}

}
