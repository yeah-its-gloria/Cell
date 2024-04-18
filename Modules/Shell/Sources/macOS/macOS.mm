// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Backend.hh"

namespace Cell::Shell::Implementations {

Wrapped<macOS*, Result> macOS::New(const String& title, const Extent extent) {
    NSRect contentRect = NSMakeRect(0, 0, extent.width, extent.height);

    NSString* nsTitle = title.IsEmpty() ? @"Cell" : title.ToPlatformNSString();

    CellWindowCreatorTrampoline* trampoline = [[CellWindowCreatorTrampoline alloc] initWithRect: contentRect];
    [trampoline performSelectorOnMainThread: @selector(create:) withObject: nsTitle waitUntilDone: YES];

    macOS* macos = new macOS(trampoline.window, trampoline.layer);

    macos->delegate = [[CellWindowDelegate alloc] initWithRefToIsDone: &macos->isDone andIsActivated: &macos->isActivated];
    [macos->window performSelectorOnMainThread: @selector(setDelegate:) withObject: macos->delegate waitUntilDone: YES];

    return macos;
}

macOS::~macOS() {
    /* ... */
}

Result macOS::RequestQuit() {
    CellWindowCreatorTrampoline* trampoline = [[CellWindowCreatorTrampoline alloc] initWithWindow: this->window andLayer: this->layer];
    [trampoline performSelectorOnMainThread: @selector(close:) withObject: nullptr waitUntilDone: YES];
    return Result::Success;
}

Result macOS::SetDrawableExtent(const Extent extent) {
    CellWindowCreatorTrampoline* trampoline = [[CellWindowCreatorTrampoline alloc] initWithWindow: this->window
        andSize: NSSize { .width = (double)extent.width, .height = (double)extent.height }];

    [trampoline performSelectorOnMainThread: @selector(resize:) withObject: nullptr waitUntilDone: YES];
    return Result::Success;
}

Result macOS::SetNewTitle(const String& title) {
    if (title.IsEmpty()) {
        return Result::InvalidParameters;
    }

    NSString* nsTitle = title.ToPlatformNSString();

    CellWindowCreatorTrampoline* trampoline = [[CellWindowCreatorTrampoline alloc] initWithWindow: this->window];
    [trampoline performSelectorOnMainThread: @selector(doTitleSet:) withObject: nsTitle waitUntilDone: YES];

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

Wrapped<Extent, Result> macOS::GetDrawableExtent() {
    CellWindowCreatorTrampoline* trampoline = [[CellWindowCreatorTrampoline alloc] initWithWindow: this->window];
    [trampoline performSelectorOnMainThread: @selector(doGetExtent:) withObject: nullptr waitUntilDone: YES];

    return Extent { .width = (uint32_t)trampoline.rect.size.width, .height = (uint32_t)trampoline.rect.size.height };
}

}
