// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Controller/Xbox.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Shell::Controller {

Wrapped<Xbox*, Result> Xbox::Find() {
    return Result::NotFound;
}

Xbox::~Xbox() {
    (void)(this->device);
}

Result Xbox::Update() {
    CELL_UNIMPLEMENTED
}

}
