// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Backend.hh"

@implementation CellWindowDelegate
-(id) initWithRefToIsDone: (bool*) isDone andIsActivated: (bool*) isActivated {
    CellWindowDelegate* obj = [super init];

    obj.isDone      = isDone;
    obj.isActivated = isActivated;

    return obj;
}

-(BOOL) windowShouldClose: (NSWindow*) sender {
    *self.isDone = true;
    return NO;
}

-(void) windowDidBecomeKey: (NSNotification *) notification {
    *self.isActivated = true;
}

-(void) windowDidResignKey: (NSNotification *) notification {
    *self.isActivated = false;
}
@end

@implementation CellWindowCreatorTrampoline
-(id) initWithRect: (NSRect) rect {
    CellWindowCreatorTrampoline* obj = [super init];

    obj.rect   = rect;
    obj.window = nil;
    obj.layer  = nil;

    return obj;
}

-(id) initWithWindow: (CellWindowImpl*) window {
    CellWindowCreatorTrampoline* obj = [super init];

    obj.rect   = NSMakeRect(0, 0, 0, 0);
    obj.window = window;
    obj.layer  = nil;

    return obj;
}

-(id) initWithWindow: (CellWindowImpl*) window andLayer: (CAMetalLayer*) layer {
    CellWindowCreatorTrampoline* obj = [super init];

    obj.rect   = NSMakeRect(0, 0, 0, 0);
    obj.window = window;
    obj.layer  = layer;

    return obj;
}

-(id) initWithWindow: (CellWindowImpl*) window andSize: (NSSize) size {
    CellWindowCreatorTrampoline* obj = [super init];

    obj.rect   = NSMakeRect(0, 0, size.width, size.height);
    obj.window = window;
    obj.layer  = nil;

    return obj;
}

-(void) create: (id) title {
    CELL_ASSERT([NSThread isMainThread] == YES);

    self.window = [[CellWindowImpl alloc] initWithContentRect: self.rect
                                          styleMask: NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskTitled
                                          backing: NSBackingStoreBuffered
                                          defer: NO];

    self.layer  = [CAMetalLayer layer];

    [self.window setReleasedWhenClosed: NO];

    [[self.window contentView] setWantsLayer: YES];
    [self.window contentView].layer = self.layer;

    [self.window setColorSpace: [NSColorSpace sRGBColorSpace]];
    [self.window setBackgroundColor: [NSColor blackColor]];
    [self.window setLevel: NSNormalWindowLevel];
    [self.window setTitle: title];

    [self.window center];

    [self.window makeKeyAndOrderFront: nil];
}

-(void) close: (id) unused {
    [self.window close];
}

-(void) resize: (id) unused {
    [self.window setContentSize: self.rect.size];
}

-(void) doTitleSet: (id) title {
    [self.window setTitle: title];
}

-(void) doGetExtent: (id) unused {
    NSView* view = [self.window contentView];

    self.rect = [view convertRectToBacking: [view bounds]];
}
@end
