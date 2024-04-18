// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/macOS.hh>

@interface CellWindowCreatorTrampoline : NSObject
-(id) initWithRect: (NSRect) rect;

-(id) initWithWindow: (CellWindowImpl*) window;
-(id) initWithWindow: (CellWindowImpl*) window andLayer: (CAMetalLayer*) layer;
-(id) initWithWindow: (CellWindowImpl*) window andSize: (NSSize) size;

-(void) create: (id) title;
-(void) close: (id) unused;
-(void) resize: (id) unused;
-(void) doTitleSet: (id) title;
-(void) doGetExtent: (id) unused;

@property(assign) NSRect rect;
@property(assign) CellWindowImpl* window;
@property(assign) CAMetalLayer* layer;
@end
