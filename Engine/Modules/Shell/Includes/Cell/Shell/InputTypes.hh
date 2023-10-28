// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Utilities/Preprocessor.hh>

namespace Cell::Shell {

// Various values for keyboards.
enum class KeyboardButton : uint64_t {
    None = 0ull,
    Zero = 1ull << 0,
    One = 1ull << 1,
    Two = 1ull << 2,
    Three = 1ull << 3,
    Four = 1ull << 4,
    Five = 1ull << 5,
    Six = 1ull << 6,
    Seven = 1ull << 7,
    Eight = 1ull << 8,
    Nine = 1ull << 9,
    A = 1ull << 10,
    B = 1ull << 11,
    C = 1ull << 12,
    D = 1ull << 13,
    E = 1ull << 14,
    F = 1ull << 15,
    G = 1ull << 16,
    H = 1ull << 17,
    I = 1ull << 18,
    J = 1ull << 19,
    K = 1ull << 20,
    L = 1ull << 21,
    M = 1ull << 22,
    N = 1ull << 23,
    O = 1ull << 24,
    P = 1ull << 25,
    Q = 1ull << 26,
    R = 1ull << 27,
    S = 1ull << 28,
    T = 1ull << 29,
    U = 1ull << 30,
    V = 1ull << 31,
    W = 1ull << 32,
    X = 1ull << 33,
    Y = 1ull << 34,
    Z = 1ull << 35
};

enum class MouseButton : uint8_t {
    None = 0,

    LeftClick = 1 << 0,
    RightClick = 1 << 1,

    // Generally represents clicking the mouse wheel itself.
    MiddleClick = 1 << 2,

    // Generally the button farthest from the thumb.
    ThumbButton1 = 1 << 3,

    // Generally the button closest to the thumb.
    ThumbButton2 = 1 << 4
};

// Axes supported by mice.
// Note that unlike controller axes, these only supply offsets, not absolute positions.
enum class MouseAxis : uint8_t {
    None = 0,

    PositionX = 1 << 0,
    PositionY = 1 << 1,
    WheelRotation = 1 << 2
};

// Various buttons on controllers.
// Based on the Xbox controller.
enum class ControllerButton : uint32_t {
    None = 0,

    // Cross on PlayStation
    // Represents A button on Switch, physically located where B/Circle would be on Xbox/PlayStation controllers
    A = 1 << 0,

    // Circle on PlayStation
    // Represents B button on Switch, physically located where A/Cross would be on Xbox/PlayStation controllers
    B = 1 << 1,

    // Square on PlayStation
    // Represents X button on Switch, physically located where Y/Triangle would be on Xbox/PlayStation controllers
    X = 1 << 2,

    // Triangle on PlayStation
    // Represents Y button on Switch, physically located where X/Square would be on Xbox/PlayStation controllers
    Y = 1 << 3,

    // Options on PlayStation, + (Plus) on Switch
    Menu = 1 << 4,

    // Create on DualSense, Share on DualShock, - (Minus) on Switch
    View = 1 << 5,

    // PS on PlayStation, HOME on Switch
    Guide = 1 << 6,

    // L3 on PlayStation
    LeftStick = 1 << 7,

    // R3 on PlayStation
    RightStick = 1 << 8,

    // L1 on PlayStation, L on Switch
    LeftBumper = 1 << 9,

    // R1 on PlayStation, R on Switch
    RightBumper = 1 << 10,

    // L2 on PlayStation, ZL on Switch
    // This isn't an actual button on Xbox or PlayStation controllers
    LeftTrigger = 1 << 11,

    // R2 on PlayStation, ZR on Switch
    // This isn't an actual button on Xbox or PlayStation controllers
    RightTrigger = 1 << 12,

    DPadUp = 1 << 13,
    DPadDown = 1 << 14,
    DPadLeft = 1 << 15,
    DPadRight = 1 << 16,

    // PlayStation only
    Touchpad = 1 << 17,
};

// Various single dimension axes (sticks, triggers) on controllers.
// Based on the Xbox controller.
enum class ControllerAxis : uint16_t {
    None = 0,

    LeftStickX = 1 << 0,
    LeftStickY = 1 << 1,

    RightStickX = 1 << 2,
    RightStickY = 1 << 3,

    // L2 on PlayStation, ZL on Switch
    // ZL on the Switch is a digital trigger, values will always be either 0 or 255.
    LeftTrigger = 1 << 4,

    // R2 on PlayStation, ZR on Switch
    // ZR on the Switch is a digital trigger, values will always be either 0 or 255.
    RightTrigger = 1 << 5,

    // PlayStation only
    TouchpadX = 1 << 6,

    // PlayStation only
    TouchpadY = 1 << 7
};

CELL_ENUM_CLASS_OPERATORS(KeyboardButton)
CELL_ENUM_CLASS_OPERATORS(MouseButton)
CELL_ENUM_CLASS_OPERATORS(MouseAxis)
CELL_ENUM_CLASS_OPERATORS(ControllerButton)
CELL_ENUM_CLASS_OPERATORS(ControllerAxis)

}
