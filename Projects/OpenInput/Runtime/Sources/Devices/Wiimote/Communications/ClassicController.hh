// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "ClassicControllerButtons.hh"

namespace Runtime::Devices::WiimoteCommunications {

struct ClassicControllerReport {
    uint8_t leftStickX;
    uint8_t rightStickX;
    uint8_t leftStickY;
    uint8_t rightStickY;
    uint8_t leftTrigger;
    uint8_t rightTrigger;
    ClassicControllerButtons buttons;
};

const uint8_t ClassicControllerIdentification[6] = {0x01, 0x00, 0xa4, 0x20, 0x01, 0x01};
const uint8_t ClassicControllerProIdentification[6] = {0x00, 0x00, 0xa4, 0x20, 0x01, 0x01};

}
