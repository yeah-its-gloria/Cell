// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/IO/HID.hh>
#include <Cell/Shell/Controller.hh>

namespace Cell::Shell::Controller {

class DualSense : public IController {
public:
    CELL_FUNCTION static Wrapped<DualSense*, Result> Find();
    CELL_FUNCTION ~DualSense();

    CELL_FUNCTION Result Update() override;

private:
    CELL_FUNCTION_INTERNAL DualSense(IO::HID::Device* d) : device(d) { }

    IO::HID::Device* device;
};

}
