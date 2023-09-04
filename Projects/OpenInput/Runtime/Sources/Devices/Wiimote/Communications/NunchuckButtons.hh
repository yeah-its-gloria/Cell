// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Runtime::Devices::WiimoteCommunications {

enum class NunchuckButtons : uint8_t {
    Z = 1 << 0,
    C = 1 << 1,

    AccelXMask = 0x0c,
    AccelYMask = 0x30,
    AccelZMask = 0xc0,

    Mask = 0x03
};

CELL_INLINE NunchuckButtons operator|(NunchuckButtons a, NunchuckButtons b) {
    return (NunchuckButtons)((uint8_t)a | (uint8_t)b);
}

CELL_INLINE NunchuckButtons& operator|=(NunchuckButtons& a, NunchuckButtons b) {
    a = a | b;
    return a;
}

CELL_INLINE NunchuckButtons operator^(NunchuckButtons a, NunchuckButtons b) {
    return (NunchuckButtons)((uint8_t)a ^ (uint8_t)b);
}

CELL_INLINE NunchuckButtons& operator^=(NunchuckButtons& a, NunchuckButtons b) {
    a = a ^ b;
    return a;
}

CELL_INLINE NunchuckButtons operator&(NunchuckButtons a, NunchuckButtons b) {
    return (NunchuckButtons)((uint8_t)a & (uint8_t)b);
}

CELL_INLINE NunchuckButtons& operator&=(NunchuckButtons& a, NunchuckButtons b) {
    a = a & b;
    return a;
}

}
