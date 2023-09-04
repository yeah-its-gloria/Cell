// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "LED.hh"
#include <Cell/System/Memory.hh>
#include <Cell/Utilities/Byteswap.hh>

namespace Runtime::Devices::WiimoteCommunications {

struct CELL_PACKED(1) RumbleRequest {
    uint8_t reportId;
    uint8_t enabled;

    // Turns on the specified LEDs.
    RumbleRequest(const bool enabled) : reportId(0x10), enabled(enabled ? 0x01 : 0x00) {}
};

struct CELL_PACKED(1) LEDsRequest {
    uint8_t reportId;
    LEDMask mask;

    // Turns on the specified LEDs.
    LEDsRequest(LEDMask mask = LEDMask::LED1) : reportId(0x11), mask(mask) {}
};

struct CELL_PACKED(1) DataReportRequest {
    uint8_t reportId;
    uint8_t parameters;
    uint8_t mode;

    DataReportRequest(const bool continuous = true, const uint8_t mode = 0x35, const bool rumble = false) : reportId(0x12), parameters(0x00), mode(mode) {
        if (rumble) {
            parameters &= 0x01;
        }

        if (continuous) {
            parameters &= 0x04;
        }
    }
};

struct CELL_PACKED(1) StatusRequest {
    uint8_t reportId;
    uint8_t unused;

    StatusRequest(const bool rumble = false) : reportId(0x15), unused(rumble ? 0xff : 0xff - 0x01) {}
};

struct CELL_PACKED(1) WriteRequest {
    uint8_t reportId;
    uint8_t space;
    uint8_t address[3];
    uint8_t size;
    uint8_t data[16];

    WriteRequest(const uint32_t address, const uint8_t size, const uint8_t data[16], const bool rumble = false)
        : reportId(0x16), space(0x04), address {0}, size(size), data {0} {
        CELL_ASSERT(size < 17);

        if (rumble) {
            space &= 0x01;
        }

        this->address[0] = address >> 16;
        this->address[1] = address >> 8;
        this->address[2] = address & 0xff;

        Cell::System::CopyMemory(this->data, data, 16);
    }

    WriteRequest(const uint32_t address, const uint8_t data, const bool rumble = false) : reportId(0x16), space(0x04), address {0}, size(1), data {0} {
        if (rumble) {
            space &= 0x01;
        }

        this->address[0] = address >> 16 & 0xff;
        this->address[1] = address >> 8 & 0xff;
        this->address[2] = address & 0xff;

        this->data[0] = data;
    }
};

struct CELL_PACKED(1) ReadRequest {
    uint8_t reportId;
    uint8_t space;
    uint8_t address[3];
    uint16_t size;

    ReadRequest(const uint32_t address, const uint16_t size, const bool rumble = false)
        : reportId(0x17), space(0x04), address {0}, size(Cell::Utilities::ByteswapBEPlatform(size)) {
        CELL_ASSERT(size < 17);

        if (rumble) {
            space &= 0x01;
        }

        this->address[0] = address >> 16;
        this->address[1] = address >> 8;
        this->address[2] = address & 0xff;
    }
};

}
