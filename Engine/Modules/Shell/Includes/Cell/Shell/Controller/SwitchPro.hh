// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/IO/HID.hh>
#include <Cell/Shell/Controller.hh>

namespace Cell::Shell::Controller {

class SwitchPro : public IController {
public:
    CELL_FUNCTION static Wrapped<SwitchPro*, Result> Find();
    CELL_FUNCTION ~SwitchPro() override;
    CELL_FUNCTION Result Update() override;

private:
    CELL_INLINE SwitchPro(IO::HID::Device* device) : device(device) { }

    CELL_FUNCTION_INTERNAL Result SubmitCommand(uint8_t cmd, uint8_t data);
    CELL_FUNCTION_INTERNAL Result SubmitUSB(uint8_t cmd, const bool waitForReply = true);

    IO::HID::Device* device;
    uint8_t commandCounter = 0;
};

}
