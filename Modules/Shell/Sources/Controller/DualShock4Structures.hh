// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Utilities/Preprocessor.hh>

namespace Cell::Shell::Controller {

enum class DualShock4Button : uint32_t {
    DPadMask = 0x0f,

    Square = 1 << 4,
    Cross = 1 << 5,
    Circle = 1 << 6,
    Triangle = 1 << 7,

    L1 = 1 << 8,
    R1 = 1 << 9,

    L2 = 1 << 10,
    R2 = 1 << 11,

    Share = 1 << 12,
    Options = 1 << 13,

    L3 = 1 << 14,
    R3 = 1 << 15,

    PS = 1 << 16,

    Touchpad = 1 << 17
};

struct CELL_PACKED(1) DualShock4ReportPacket {
    uint8_t id;
    uint8_t leftStickX;
    uint8_t leftStickY;
    uint8_t rightStickX;
    uint8_t rightStickY;
    uint8_t buttonData[3];
    uint8_t leftTrigger;
    uint8_t rightTrigger;
    uint16_t timestamp;
    uint8_t unknown1;
    uint16_t gyroscope[3];
    uint16_t accelerometer[3];
    uint8_t unknown2[5];
    uint8_t batteryLevel;
    uint8_t unknown3[4];
    uint8_t touchpadCounter1;
    uint8_t touchpadData1[3];
    uint8_t touchpadCounter2;
    uint8_t touchpadData2[3];
    uint8_t padding[21];
};

struct CELL_PACKED(1) DualShock4EffectsPacket {
    uint8_t id;
    uint8_t magic;
    uint16_t padding1;
    uint8_t rumbleRight;
    uint8_t rumbleLeft;
    uint8_t ledRed;
    uint8_t ledGreen;
    uint8_t ledBlue;
    uint8_t unknown[10];
    uint8_t volumes[4];
    uint8_t padding2[9];
};

CELL_ENUM_CLASS_OPERATORS(DualShock4Button);

}
