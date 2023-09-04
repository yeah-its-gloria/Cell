// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Runtime::Devices::WiimoteCommunications {

enum class LEDMask : uint8_t {
    EnableRumble = 0x01,

    LED1 = 1 << 4,
    LED2 = 1 << 5,
    LED3 = 1 << 6,
    LED4 = 1 << 7
};

CELL_INLINE LEDMask operator|(LEDMask a, LEDMask b) {
    return (LEDMask)((uint8_t)a | (uint8_t)b);
}

CELL_INLINE LEDMask& operator|=(LEDMask& a, LEDMask b) {
    a = a | b;
    return a;
}

CELL_INLINE LEDMask operator^(LEDMask a, LEDMask b) {
    return (LEDMask)((uint8_t)a ^ (uint8_t)b);
}

CELL_INLINE LEDMask& operator^=(LEDMask& a, LEDMask b) {
    a = a ^ b;
    return a;
}

CELL_INLINE LEDMask operator&(LEDMask a, LEDMask b) {
    return (LEDMask)((uint8_t)a & (uint8_t)b);
}

CELL_INLINE LEDMask& operator&=(LEDMask& a, LEDMask b) {
    a = a & b;
    return a;
}

}
