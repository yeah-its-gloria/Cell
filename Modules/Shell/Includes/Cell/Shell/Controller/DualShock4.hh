// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/IO/HID.hh>
#include <Cell/Shell/Controller.hh>

namespace Cell::Shell::Controller {

class DualShock4 : public IController {
public:
    CELL_FUNCTION static Wrapped<DualShock4*, Result> Find();
    CELL_FUNCTION ~DualShock4() override;
    CELL_FUNCTION Result Update() override;

    CELL_NON_COPYABLE(DualShock4)

private:
    CELL_INLINE DualShock4(IO::HID::Device* device) : device(device) { }

    IO::HID::Device* device;
};

}
