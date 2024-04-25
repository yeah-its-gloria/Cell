// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Cell/System/Panic.hh"
#include <Cell/System/Entry.hh>
#include <Cell/System/Thread.hh>
#include <Cell/System/Timer.hh>

#include <AppKit/AppKit.h>
#include <Foundation/NSThread.h>

#include <CoreFoundation/CoreFoundation.h>

#include <stdio.h>

using namespace Cell;

@interface CellAppDelegate : NSObject<NSApplicationDelegate>
// ...
@end

@implementation CellAppDelegate
-(NSApplicationTerminateReply) applicationShouldTerminate: (NSApplication*) app {
    (void)(app);

	return NSTerminateNow;
}

-(BOOL) applicationSupportsSecureRestorableState: (NSApplication*) app {
    (void)(app);

    return FALSE;
}
@end

int main() {
    const int result = setvbuf(stdout, nullptr, _IONBF, 0);
    CELL_ASSERT(result == 0);

    NSApplication* application = [NSApplication sharedApplication];

    CellAppDelegate* delegate = [[CellAppDelegate alloc] init];
    [application setActivationPolicy: NSApplicationActivationPolicyRegular];
    [application setDelegate: delegate];

    const BOOL multithreaded = [NSThread isMultiThreaded];
    if (multithreaded == NO) {
        [NSThread detachNewThreadSelector: @selector(class) toTarget: [NSObject class] withObject: nullptr];
        CELL_ASSERT([NSThread isMultiThreaded] == YES);
    }

    NSMenu*     menuBar      = [[NSMenu alloc] init];
    NSMenu*     appMenu      = [[NSMenu alloc] init];
    NSMenuItem* appMenuItem  = [[NSMenuItem alloc] init];
    NSMenuItem* quitMenuItem = [[NSMenuItem alloc] initWithTitle: @"Quit" action: @selector(terminate:) keyEquivalent: @"q"];

    [appMenu     addItem:     quitMenuItem];
    [appMenuItem setSubmenu:  appMenu];
    [menuBar     addItem:     appMenuItem];
    [application setMainMenu: menuBar];

    [application finishLaunching];
    [application activate];

    String parameters = "";

    Reference ref(parameters); // what did I even need references for
    System::Thread mainThread(([](void* p) { Reference<String> r = *(Reference<String>*)p; CellEntry(r); }), (void*)&ref, "Main Thread");

    while (mainThread.IsActive()) {
        System::Sleep(1);

        NSEvent* event = [application nextEventMatchingMask: NSEventMaskAny
                                      untilDate: [NSDate distantPast]
                                      inMode: NSDefaultRunLoopMode
                                      dequeue: YES];
        if (event == nullptr) {
            continue;
        }

        [application sendEvent: event];
    }

    return 0;
}
