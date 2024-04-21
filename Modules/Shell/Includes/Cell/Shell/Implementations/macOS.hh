// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Shell/Shell.hh>

#ifdef __OBJC__
#include <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>

@interface CellWindowImpl : NSWindow {}
@property(assign) Cell::Shell::KeyboardButton* keysRef;
@end

@interface CellWindowDelegate : NSObject<NSWindowDelegate>
-(id) initWithRefToIsDone: (bool*) isDone andIsActivated: (bool*) isActivated;

-(BOOL) windowShouldClose: (NSWindow*) sender;
-(void) windowDidBecomeKey: (NSNotification *) notification;
-(void) windowDidResignKey: (NSNotification *) notification;

@property(assign) bool* isDone;
@property(assign) bool* isActivated;
@end
#else
typedef void CellWindowImpl;
typedef void CellWindowDelegate;
typedef void CAMetalLayer;
#endif

namespace Cell::Shell::Implementations {

class macOS : public IShell {
public:
    // Sets up an AppKit/Cocoa client.
    CELL_FUNCTION static Wrapped<macOS*, Result> New(const String& title, const Extent extent);
    CELL_FUNCTION ~macOS();

    CELL_NODISCARD CELL_FUNCTION_TEMPLATE CAMetalLayer* GetMetalLayer() const { return this->layer; }
    CELL_NODISCARD CELL_FUNCTION_TEMPLATE CellWindowImpl* GetWindow() const { return this->window; }

    CELL_FUNCTION Result RequestQuit() override;
    CELL_FUNCTION Wrapped<Extent, Result> GetDrawableExtent() override;
    CELL_FUNCTION Result SetDrawableExtent(const Extent extent) override;
    CELL_FUNCTION Result SetNewTitle(const String& title) override;
    CELL_FUNCTION Result IndicateStatus(const ShellStatus status) override;
    CELL_FUNCTION Result CaptureState(const bool captured) override;
    CELL_FUNCTION Result Log(const String& text) override;
    CELL_FUNCTION Result LogClear() override;

private:
    CELL_FUNCTION_INTERNAL macOS(CellWindowImpl* w, CAMetalLayer* l) : window(w), layer(l), delegate(nullptr) { }

    CELL_FUNCTION_INTERNAL Result RunDispatchImpl() override;

    CellWindowImpl* window;
    CAMetalLayer* layer;
    CellWindowDelegate* delegate;
};

}
