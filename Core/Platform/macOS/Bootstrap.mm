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
    NSWindow* window = [app mainWindow];
    if (window != nullptr) {
        [window performClose: self];

        return NSTerminateCancel;
    }

	return NSTerminateNow;
}

-(BOOL) applicationSupportsSecureRestorableState: (NSApplication*) app {
    (void)(app);

    return NO;
}
@end

int main() {
    const int stdResult = setvbuf(stdout, nullptr, _IONBF, 0);
    CELL_ASSERT(stdResult == 0);

    NSApplication* app = [NSApplication sharedApplication];

    CellAppDelegate* delegate = [CellAppDelegate new];
    [app setActivationPolicy: NSApplicationActivationPolicyRegular];
    [app setDelegate: delegate];

    BOOL result = [NSThread isMultiThreaded];
    if (result == NO) {
        [NSThread detachNewThreadSelector: @selector(class) toTarget: [NSObject class] withObject: nullptr];

        result = [NSThread isMultiThreaded];
        CELL_ASSERT(result == YES);
    }

    NSBundle* bundle = [NSBundle mainBundle];
    if (bundle != nullptr && [bundle bundleIdentifier] != nullptr) {
        result = [[NSFileManager defaultManager] changeCurrentDirectoryPath: [bundle resourcePath]];
        CELL_ASSERT(result == YES);
    }

    NSMenu*     menuBar      = [NSMenu new];
    NSMenu*     appMenu      = [NSMenu new];
    NSMenuItem* appMenuItem  = [NSMenuItem new];
    NSMenuItem* quitMenuItem = [[NSMenuItem alloc] initWithTitle: @"Quit" action: @selector(terminate:) keyEquivalent: @"q"];

    [appMenu     addItem:     quitMenuItem];
    [appMenuItem setSubmenu:  appMenu];
    [menuBar     addItem:     appMenuItem];
    [app         setMainMenu: menuBar];

    [app finishLaunching];

    String parameters = "";

    Reference ref(parameters); // what did I even need references for
    System::Thread mainThread(([](void* p) { Reference<String> r = *(Reference<String>*)p; CellEntry(r); }), (void*)&ref, "Main Thread");

    while (mainThread.IsActive()) {
        System::Sleep(1);

        NSEvent* event = [app nextEventMatchingMask: NSEventMaskAny
                              untilDate:             [NSDate distantPast]
                              inMode:                NSDefaultRunLoopMode
                              dequeue:               YES];

        if (event == nullptr) {
            continue;
        }

        [app sendEvent: event];
    }

    return 0;
}
