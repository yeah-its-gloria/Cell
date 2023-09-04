// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Runtime::Devices::WiimoteCommunications {

enum class Buttons : uint16_t {
    DPadLeft = 1 << 0,
    DPADRight = 1 << 1,
    DPADDown = 1 << 2,
    DPADUp = 1 << 3,
    Plus = 1 << 4,
    Two = 1 << 8,
    One = 1 << 9,
    B = 1 << 10,
    A = 1 << 11,
    HOME = 1 << 15
};

CELL_INLINE Buttons operator|(Buttons a, Buttons b) {
    return (Buttons)((uint16_t)a | (uint16_t)b);
}

CELL_INLINE Buttons& operator|=(Buttons& a, Buttons b) {
    a = a | b;
    return a;
}

CELL_INLINE Buttons operator^(Buttons a, Buttons b) {
    return (Buttons)((uint16_t)a ^ (uint16_t)b);
}

CELL_INLINE Buttons& operator^=(Buttons& a, Buttons b) {
    a = a ^ b;
    return a;
}

CELL_INLINE Buttons operator&(Buttons a, Buttons b) {
    return (Buttons)((uint16_t)a & (uint16_t)b);
}

CELL_INLINE Buttons& operator&=(Buttons& a, Buttons b) {
    a = a & b;
    return a;
}

}
