// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Runtime::Devices {

enum class GameCubeControllerButtons : uint16_t {
    A = 1 << 0,
    B = 1 << 1,
    X = 1 << 2,
    Y = 1 << 3,

    DPadLeft = 1 << 4,
    DPadRight = 1 << 5,
    DPadDown = 1 << 6,
    DPadUp = 1 << 7,

    Start = 1 << 8,
    Z = 1 << 9,
    R = 1 << 10,
    L = 1 << 11,
};

CELL_INLINE GameCubeControllerButtons operator|(GameCubeControllerButtons a, GameCubeControllerButtons b) {
    return (GameCubeControllerButtons)((uint16_t)a | (uint16_t)b);
}

CELL_INLINE GameCubeControllerButtons& operator|=(GameCubeControllerButtons& a, GameCubeControllerButtons b) {
    a = a | b;
    return a;
}

CELL_INLINE GameCubeControllerButtons operator^(GameCubeControllerButtons a, GameCubeControllerButtons b) {
    return (GameCubeControllerButtons)((uint16_t)a ^ (uint16_t)b);
}

CELL_INLINE GameCubeControllerButtons& operator^=(GameCubeControllerButtons& a, GameCubeControllerButtons b) {
    a = a ^ b;
    return a;
}

CELL_INLINE GameCubeControllerButtons operator&(GameCubeControllerButtons a, GameCubeControllerButtons b) {
    return (GameCubeControllerButtons)((uint16_t)a & (uint16_t)b);
}

CELL_INLINE GameCubeControllerButtons& operator&=(GameCubeControllerButtons& a, GameCubeControllerButtons b) {
    a = a & b;
    return a;
}

struct CELL_PACKED(1) GameCubeControllerReport {
    uint8_t status;
    GameCubeControllerButtons buttons;
    uint8_t controlStickX;
    uint8_t controlStickY;
    uint8_t cameraStickX;
    uint8_t cameraStickY;
    uint8_t triggerLeft;
    uint8_t triggerRight;
};

}