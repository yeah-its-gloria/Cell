// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/macOS.hh>

// TODO: cleanup and testing!!

@implementation CellWindowImpl
-(BOOL) canBecomeKeyWindow {
    return YES;
}

-(BOOL) canBecomeMainWindow {
    return YES;
}
@end

@interface CellWindowCreatorTrampoline : NSObject
-(id) initWithTitle: (NSString*) title andRect: (NSRect) rect;

-(id) initWithWindow: (CellWindowImpl*) window;
-(id) initWithWindow: (CellWindowImpl*) window andLayer: (CAMetalLayer*) layer;
-(id) initWithWindow: (CellWindowImpl*) window andSize: (NSSize) size;
-(id) initWithWindow: (CellWindowImpl*) window andTitle: (NSString*) title;

-(void) create: (id) unused;
-(void) destruct: (id) unused;
-(void) close: (id) unused;
-(void) resize: (id) unused;
-(void) doTitleSet: (id) unused;
-(void) doGetExtent: (id) unused;

@property(assign) NSString* title;
@property(assign) NSRect rect;
@property(assign) CellWindowImpl* window;
@property(assign) CAMetalLayer* layer;
@end

@implementation CellWindowCreatorTrampoline
-(id) initWithTitle: (NSString*) title andRect: (NSRect) rect {
    CellWindowCreatorTrampoline* obj = [super init];

    obj.title  = title;
    obj.rect   = rect;
    obj.window = nil;
    obj.layer  = nil;

    return obj;
}

-(id) initWithWindow: (CellWindowImpl*) window {
    CellWindowCreatorTrampoline* obj = [super init];

    obj.title  = nil;
    obj.rect   = NSMakeRect(0, 0, 0, 0);
    obj.window = window;
    obj.layer  = nil;

    return obj;
}

-(id) initWithWindow: (CellWindowImpl*) window andLayer: (CAMetalLayer*) layer {
    CellWindowCreatorTrampoline* obj = [super init];

    obj.title  = nil;
    obj.rect   = NSMakeRect(0, 0, 0, 0);
    obj.window = window;
    obj.layer  = layer;

    return obj;
}

-(id) initWithWindow: (CellWindowImpl*) window andSize: (NSSize) size {
    CellWindowCreatorTrampoline* obj = [super init];

    obj.title  = nil;
    obj.rect   = NSMakeRect(0, 0, size.width, size.height);
    obj.window = window;
    obj.layer  = nil;

    return obj;
}

-(id) initWithWindow: (CellWindowImpl*) window andTitle: (NSString*) title {
    CellWindowCreatorTrampoline* obj = [super init];

    obj.title  = title;
    obj.rect   = NSMakeRect(0, 0, 0, 0);
    obj.window = window;
    obj.layer  = nil;

    return obj;
}

-(void) create: (id) unused {
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
    [self.window setTitle: self.title];

    [self.window center];

    [self.window makeKeyAndOrderFront: nil];
}

-(void) destruct: (id) unused {
    [self.window dealloc];
    [self.layer dealloc];
}

-(void) close: (id) unused {
    [self.window close];
}

-(void) resize: (id) unused {
    [self.window setContentSize: self.rect.size];
}

-(void) doTitleSet: (id) unused {
    [self.window setTitle: self.title];
}

-(void) doGetExtent: (id) unused {
    NSView* view = [self.window contentView];

    self.rect = [view convertRectToBacking: [view bounds]];
}
@end

namespace Cell::Shell::Implementations {

Wrapped<macOS*, Result> macOS::New(const String& title, const Extent extent) {
    @autoreleasepool {
        NSRect contentRect = NSMakeRect(0, 0, extent.width, extent.height);

        NSString* nsTitle = nullptr;
        if (title.IsEmpty()) {
            nsTitle =  @"Cell";
        } else {
            nsTitle = [[NSString alloc] initWithBytes: title.ToRawPointer() length: title.GetSize() encoding: NSUTF8StringEncoding];
        }

        CellWindowCreatorTrampoline* trampoline = [[CellWindowCreatorTrampoline alloc] initWithTitle: nsTitle andRect: contentRect];
        [trampoline performSelectorOnMainThread: @selector(create:) withObject: nil waitUntilDone: YES];

        return new macOS(trampoline.window, trampoline.layer);
    }
}

macOS::~macOS() {
    @autoreleasepool {
        CellWindowCreatorTrampoline* trampoline = [[CellWindowCreatorTrampoline alloc] initWithWindow: this->impl andLayer: this->layer];
        [trampoline performSelectorOnMainThread: @selector(destruct:) withObject: nil waitUntilDone: YES];
    }
}

Result macOS::RequestQuit() {
    @autoreleasepool {
        CellWindowCreatorTrampoline* trampoline = [[CellWindowCreatorTrampoline alloc] initWithWindow: this->impl andLayer: nil];
        [trampoline performSelectorOnMainThread: @selector(close:) withObject: nil waitUntilDone: YES];
    }

    return Result::Success;
}

Result macOS::SetDrawableExtent(const Extent extent) {
    @autoreleasepool {
        CellWindowCreatorTrampoline* trampoline = [[CellWindowCreatorTrampoline alloc] initWithWindow: this->impl
                                                                                       andSize: NSSize { .width = (double)extent.width, .height = (double)extent.height }];

        [trampoline performSelectorOnMainThread: @selector(resize:) withObject: nil waitUntilDone: YES];
    }

    return Result::Success;
}

Result macOS::SetNewTitle(const String& title) {
    if (title.IsEmpty()) {
        return Result::InvalidParameters;
    }

    @autoreleasepool {
        NSString* nsTitle = [[NSString alloc] initWithBytes: title.ToRawPointer() length: title.GetSize() encoding: NSUTF8StringEncoding];

        CellWindowCreatorTrampoline* trampoline = [[CellWindowCreatorTrampoline alloc] initWithWindow: this->impl andTitle: nsTitle];
        [trampoline performSelectorOnMainThread: @selector(doTitleSet:) withObject: nil waitUntilDone: YES];
    }

    return Result::Success;
}

Result macOS::IndicateStatus(const ShellStatus status) {
    switch (status) {
    case ShellStatus::Default:
    case ShellStatus::Working: {
        break;
    }

    default: {
        CELL_UNIMPLEMENTED
    }
    }

    return Result::Success;
}

Result macOS::CaptureState(const bool captured) {
    (void)(captured);

    return Result::Success;
}

Result macOS::Log(const String& text) {
    (void)(text);

    return Result::Success;
}

Result macOS::LogClear() {
    return Result::Success;
}

Result macOS::RunDispatchImpl() {
    /*@autoreleasepool {
        this->isDone      = [this->impl isVisible]   == NO;
        this->isActivated = [this->impl isKeyWindow] == YES;
    }*/

    return Result::Success;
}

Wrapped<Extent, Result> macOS::GetDrawableExtent() {
    Extent extent { 0, 0 };

    @autoreleasepool {
        CellWindowCreatorTrampoline* trampoline = [[CellWindowCreatorTrampoline alloc] initWithWindow: this->impl];
        [trampoline performSelectorOnMainThread: @selector(doGetExtent:) withObject: nil waitUntilDone: YES];

        extent.width  = trampoline.rect.size.width;
        extent.height = trampoline.rect.size.height;
    }

    return extent;
}

}
