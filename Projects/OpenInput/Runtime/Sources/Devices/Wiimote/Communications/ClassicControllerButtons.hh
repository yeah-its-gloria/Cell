// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Runtime::Devices::WiimoteCommunications {

enum class ClassicControllerButtons : uint16_t {
    R = 1 << 1,
    Plus = 1 << 2,
    HOME = 1 << 3,
    Minus = 1 << 4,
    L = 1 << 5,
    DPadDown = 1 << 6,
    DPadRight = 1 << 7,
    DPadUp = 1 << 8,
    DPadLeft = 1 << 9,
    ZR = 1 << 10,
    x = 1 << 11,
    a = 1 << 12,
    y = 1 << 13,
    b = 1 << 14,
    ZL = 1 << 15,

    Mask = 0xffff
};

CELL_INLINE ClassicControllerButtons operator|(ClassicControllerButtons a, ClassicControllerButtons b) {
    return (ClassicControllerButtons)((uint16_t)a | (uint16_t)b);
}

CELL_INLINE ClassicControllerButtons& operator|=(ClassicControllerButtons& a, ClassicControllerButtons b) {
    a = a | b;
    return a;
}

CELL_INLINE ClassicControllerButtons operator^(ClassicControllerButtons a, ClassicControllerButtons b) {
    return (ClassicControllerButtons)((uint16_t)a ^ (uint16_t)b);
}

CELL_INLINE ClassicControllerButtons& operator^=(ClassicControllerButtons& a, ClassicControllerButtons b) {
    a = a ^ b;
    return a;
}

CELL_INLINE ClassicControllerButtons operator&(ClassicControllerButtons a, ClassicControllerButtons b) {
    return (ClassicControllerButtons)((uint16_t)a & (uint16_t)b);
}

CELL_INLINE ClassicControllerButtons& operator&=(ClassicControllerButtons& a, ClassicControllerButtons b) {
    a = a & b;
    return a;
}

}
