// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "Buttons.hh"
#include "StatusState.hh"

namespace Runtime::Devices::WiimoteCommunications {

struct CELL_PACKED(1) BaseReport {
    uint8_t reportId;
    uint8_t data[21];
};

struct CELL_PACKED(1) StatusReport {
    uint8_t reportId;// 0x20
    Buttons buttons;
    StatusState state;
    uint16_t unknown;
    uint8_t batteryLevel;
    uint8_t padding[15];
};

struct CELL_PACKED(1) ReadReport {
    uint8_t reportId;// 0x21
    Buttons buttons;
    uint8_t sizeAndError;
    uint16_t offset;
    uint8_t data[16];
};

struct CELL_PACKED(1) AcknowledgeReport {
    uint8_t reportId;// 0x22
    Buttons buttons;
    uint8_t acknowledgedReportId;
    uint8_t result;
    uint8_t padding[17];
};

struct CELL_PACKED(1) ButtonsAccelExtensionReport {
    uint8_t reportId;// 0x35
    Buttons buttons;
    uint8_t accelerometerData[3];
    uint8_t extensionData[16];
};

}
