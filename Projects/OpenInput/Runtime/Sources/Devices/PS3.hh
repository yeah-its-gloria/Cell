// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Runtime::Devices::PlayStation3 {

enum class Buttons : uint32_t {
    Select = 1 << 0,
    L3 = 1 << 1,
    R3 = 1 << 2,
    Start = 1 << 3,
    DPADUp = 1 << 4,
    DPADRight = 1 << 5,
    DPADDown = 1 << 6,
    DPADLeft = 1 << 7,
    L2 = 1 << 8,
    R2 = 1 << 9,
    L1 = 1 << 10,
    R1 = 1 << 11,
    Triangle = 1 << 12,
    Circle = 1 << 13,
    Cross = 1 << 14,
    Square = 1 << 15,
    PS = 1 << 16
};

struct CELL_PACKED(1) DataReport {
    uint8_t ReportId;

    uint8_t Reserved0;

    Buttons DigitalButtons;

    uint8_t LeftThumbX;
    uint8_t LeftThumbY;
    uint8_t RightThumbX;
    uint8_t RightThumbY;

    uint8_t Reserved1[4];

    uint8_t Up;
    uint8_t Right;
    uint8_t Down;
    uint8_t Left;

    uint8_t L2;
    uint8_t R2;

    uint8_t L1;
    uint8_t R1;

    uint8_t Triangle;
    uint8_t Circle;
    uint8_t Cross;
    uint8_t Square;

    uint8_t Reserved2[4];

    uint8_t BatteryStatus;

    uint8_t Reserved3[10];

    uint32_t AccelerometerX;
    uint32_t AccelerometerY;
};

const uint16_t VendorId = 0x054c;
const uint16_t ProductId = 0x0268;

}::PlayStation3
