// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Runtime::Devices::WiimoteCommunications {

enum class StatusState : uint8_t {
    AlmostEmpty = 1 << 0,
    ExtensionConnected = 1 << 1,
    SpeakerEnabled = 1 << 2,
    CameraEnabled = 1 << 3,
    LED1Enabled = 1 << 4,
    LED2Enabled = 1 << 5,
    LED3Enabled = 1 << 6,
    LED4Enabled = 1 << 7,
};

CELL_INLINE StatusState operator|(StatusState a, StatusState b) {
    return (StatusState)((uint8_t)a | (uint8_t)b);
}

CELL_INLINE StatusState& operator|=(StatusState& a, StatusState b) {
    a = a | b;
    return a;
}

CELL_INLINE StatusState operator^(StatusState a, StatusState b) {
    return (StatusState)((uint8_t)a ^ (uint8_t)b);
}

CELL_INLINE StatusState& operator^=(StatusState& a, StatusState b) {
    a = a ^ b;
    return a;
}

CELL_INLINE StatusState operator&(StatusState a, StatusState b) {
    return (StatusState)((uint8_t)a & (uint8_t)b);
}

CELL_INLINE StatusState& operator&=(StatusState& a, StatusState b) {
    a = a & b;
    return a;
}

}
