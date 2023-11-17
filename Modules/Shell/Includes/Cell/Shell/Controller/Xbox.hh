// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/Shell/Controller.hh>

namespace Cell::Shell::Controller {

class Xbox : public IController {
public:
    CELL_FUNCTION static Wrapped<Xbox*, Result> Find();
    CELL_FUNCTION ~Xbox() override;
    CELL_FUNCTION Result Update() override;

private:
    CELL_INLINE Xbox(uintptr_t device) : device(device) { }

    uintptr_t device;
};

}
