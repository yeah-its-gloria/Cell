// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/macOS.hh>

namespace Cell::Shell::Implementations {

Wrapped<macOS*, Result> macOS::New(const String& title, const Extent extent) {
    NSString* nsTitle = title.IsEmpty() ? @"Cell" : title.ToPlatformNSString();

    macOS* instance = new macOS();
    instance->delegate = [[CellWindowDelegate alloc] initWithRefToIsDone: &instance->isDone andIsActivated: &instance->isActivated];

    dispatch_sync(dispatch_get_main_queue(), ^{
        instance->window = [[CellWindowImpl alloc] initWithContentRect: NSMakeRect(0, 0, extent.width, extent.height)
                                                   styleMask: NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskTitled
                                                   backing: NSBackingStoreBuffered
                                                   defer: NO];

        instance->layer = [CAMetalLayer layer];

        [instance->window setReleasedWhenClosed: NO];

        NSView* view = [instance->window contentView];
        [view setWantsLayer: YES];
        view.layer = instance->layer;

        [instance->window setColorSpace: [NSColorSpace sRGBColorSpace]];
        [instance->window setBackgroundColor: [NSColor blackColor]];
        [instance->window setLevel: NSNormalWindowLevel];
        [instance->window setTitle: nsTitle];
        [instance->window setDelegate: instance->delegate];

        [instance->window center];
        [instance->window makeKeyAndOrderFront: nullptr];
    });

    instance->window.keysRef = &instance->keys;
    return instance;
}

macOS::~macOS() {
    // ...
}

Result macOS::RequestQuit() {
    dispatch_sync(dispatch_get_main_queue(), ^{
        [this->window performClose: nullptr];
    });

    return Result::Success;
}

Wrapped<Extent, Result> macOS::GetDrawableExtent() {
    __block CGSize size = { 0.f, 0.f };

    dispatch_sync(dispatch_get_main_queue(), ^{
        NSView* view = [this->window contentView];
        size = [view convertRectToBacking: [view bounds]].size;
    });

    return Extent { .width = (uint32_t)size.width, .height = (uint32_t)size.height };
}

Result macOS::SetDrawableExtent(const Extent extent) {
    dispatch_sync(dispatch_get_main_queue(), ^{
        [this->window setContentSize: CGSize { .width = (CGFloat)extent.width, .height = (CGFloat)extent.height }];
    });

    return Result::Success;
}

Result macOS::SetNewTitle(const String& title) {
    if (title.IsEmpty()) {
        return Result::InvalidParameters;
    }

    NSString* nsTitle = title.ToPlatformNSString();
    dispatch_sync(dispatch_get_main_queue(), ^{
        [this->window setTitle: nsTitle];
    });

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
    // ...

    return Result::Success;
}

}
