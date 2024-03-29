// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/Shell/Controller.hh>

namespace Cell::Shell::Controller {

class Xbox : public IController {
public:
    CELL_FUNCTION static Wrapped<Xbox*, Result> Find();
    CELL_FUNCTION ~Xbox();

    CELL_FUNCTION Result Update() override;

    CELL_NON_COPYABLE(Xbox)

private:
    CELL_INLINE Xbox(uintptr_t device) : device(device) { }

    uintptr_t device;
};

}
