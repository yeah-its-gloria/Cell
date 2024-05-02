// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Controller/Xbox.hh>
#include <Cell/System/Panic.hh>

#include <GameController/GameController.h>

namespace Cell::Shell::Controller {

Wrapped<Xbox*, Result> Xbox::Find() {
    // TODO: move GameController to a separate class, allow direct access to Xbox controllers, properly iterate controller

    NSArray<GCController*>* controllers = [GCController controllers];

    NSEnumerator<GCController*>* enumerator = [controllers objectEnumerator];
    while (true) {
        GCController* controller = [enumerator nextObject];
        if (controller == nullptr) {
            break;
        }

        if (controller.productCategory == GCProductCategoryXboxOne) {
            return new Xbox((uintptr_t)controller);
        }
    }

    return Result::NotFound;
}

Xbox::~Xbox() {
    [(GCController*)this->device release];
}

Result Xbox::Update() {
    //GCController* controller = (GCController*)this->device;
    //[[controller extendedGamepad] allButtons];

    return Result::Success;
}

}
