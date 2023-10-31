// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/IO/HID.hh>
#include <Cell/Shell/Controller.hh>

namespace Cell::Shell::Controller {

struct DualSenseProperties {
    uint32_t ledData; // RGBb (brightness)

    uint8_t rumbleLeft;
    uint8_t rumbleRight;

    bool playerLedOn;
};

class DualSense : public IController {
public:
    CELL_FUNCTION static Wrapped<DualSense*, Result> Find();
    CELL_FUNCTION ~DualSense() override;
    CELL_FUNCTION Result Update() override;

    CELL_INLINE void UpdateProperties(DualSenseProperties properties) {
        this->properties = properties;
        this->hasUpdated = false;
    }

private:
    CELL_INLINE DualSense(IO::HID* device) : device(device) { }

    IO::HID* device;

    DualSenseProperties properties = { };
    bool hasUpdated = false;
};

}
