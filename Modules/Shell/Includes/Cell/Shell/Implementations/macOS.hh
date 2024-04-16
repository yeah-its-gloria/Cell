// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Shell/Shell.hh>

#ifdef __OBJC__
#include <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>

@interface CellWindowImpl : NSWindow {}
@end
#else
typedef void CellWindowImpl;
typedef void CAMetalLayer;
#endif

namespace Cell::Shell::Implementations {

class macOS : public IShell {
public:
    // Sets up an AppKit/Cocoa client.
    CELL_FUNCTION static Wrapped<macOS*, Result> New(const String& title, const Extent extent);
    CELL_FUNCTION ~macOS();

    CELL_FUNCTION_TEMPLATE CAMetalLayer* GetMetalLayer() { return this->layer; }

    CELL_FUNCTION Result RequestQuit() override;
    CELL_FUNCTION Wrapped<Extent, Result> GetDrawableExtent() override;
    CELL_FUNCTION Result SetDrawableExtent(const Extent extent) override;
    CELL_FUNCTION Result SetNewTitle(const String& title) override;
    CELL_FUNCTION Result IndicateStatus(const ShellStatus status) override;
    CELL_FUNCTION Result CaptureState(const bool captured) override;
    CELL_FUNCTION Result Log(const String& text) override;
    CELL_FUNCTION Result LogClear() override;

private:
    CELL_FUNCTION_INTERNAL macOS(CellWindowImpl* i, CAMetalLayer* l) : impl(i), layer(l) { }

    CELL_FUNCTION_INTERNAL Result RunDispatchImpl() override;

    CellWindowImpl* impl;
    CAMetalLayer* layer;
};

}