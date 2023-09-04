// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "NunchuckButtons.hh"

namespace Runtime::Devices::WiimoteCommunications {

struct NunchuckReport {
    uint8_t stickX;
    uint8_t stickY;
    uint8_t accelX;
    uint8_t accelY;
    uint8_t accelZ;
    NunchuckButtons buttons;
};

const uint8_t NunchuckIdentification[6] = {0xff, 0x00, 0xa4, 0x20, 0x00, 0x00};

}
