// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/IO/HID.hh>
#include <Cell/Shell/Controller.hh>

namespace Cell::Shell::Controller {

class DualShock4 : public IController {
public:
    CELL_FUNCTION static Wrapped<DualShock4*, Result> Find();
    CELL_FUNCTION ~DualShock4();

    CELL_FUNCTION Result Update() override;

private:
    CELL_FUNCTION_INTERNAL DualShock4(IO::HID::Device* d) : device(d) { }

    IO::HID::Device* device;
};

}
