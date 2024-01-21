// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Controller/Xbox.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Shell::Controller {

Wrapped<Xbox*, Result> Xbox::Find() {
    //CELL_UNIMPLEMENTED

    return Result::NotFound;
}

Xbox::~Xbox() {
    (void)(this->device);

    //CELL_UNIMPLEMENTED
}

Result Xbox::Update() {
    CELL_UNIMPLEMENTED
}

}
