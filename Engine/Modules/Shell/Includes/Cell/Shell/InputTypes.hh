// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Utilities/Preprocessor.hh>

namespace Cell::Shell {

enum class InputType : uint8_t {
    Pressed,
    Held,
    Released,
};

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
    Z = 1ull << 35,

    Escape = 1ull << 36,
    Space = 1ull << 37,
    Backspace = 1ull << 38,
    Tab = 1ull << 39,
    Return = 1ull << 40,
    LeftShift = 1ull << 41,
    RightShift = 1ull << 42,
    LeftControl = 1ull << 43,
    RightControl = 1ull << 44,
    LeftAlt = 1ull << 45,
    RightAlt = 1ull << 46,

    UpArrow = 1ull << 47,
    DownArrow = 1ull << 48,
    LeftArrow = 1ull << 49,
    RightArrow = 1ull << 50,

    F1 = 1ull << 51,
    F2 = 1ull << 52,
    F3 = 1ull << 53,
    F4 = 1ull << 54,
    F5 = 1ull << 55,
    F6 = 1ull << 56,
    F7 = 1ull << 57,
    F8 = 1ull << 58,
    F9 = 1ull << 59,
    F10 = 1ull << 60,
    F11 = 1ull << 61,
    F12 = 1ull << 62
};

enum class MouseButton : uint8_t {
    MouseLeftClick = 1 << 0,
    MouseRightClick = 1 << 1,

    // Generally represents clicking the mouse wheel itself.
    MouseMiddleClick = 1 << 2,

    // Generally the button farthest from the thumb.
    MouseThumbButton1 = 1 << 3,

    // Generally the button closest to the thumb.
    MouseThumbButton2 = 1 << 4,
};

enum class ControllerButton : uint32_t {
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

    // Switch only
    Capture = 1 << 18
};

enum class MouseAxis : uint8_t {
    None = 0,

    // Only provides an offset.
    PositionX = 1 << 0,

    // Only provides an offset.
    PositionY = 1 << 1,

    // Only provides an offset.
    WheelRotation = 1 << 2
};

enum class ControllerAxis : uint8_t {
    LeftStickX = 1ull << 0,
    LeftStickY = 1ull << 1,

    RightStickX = 1ull << 2,
    RightStickY = 1ull << 3,

    // L2 on PlayStation, ZL on Switch
    // ZL on the Switch is a digital trigger, values will always be either 0 or 255.
    LeftTrigger = 1ull << 4,

    // R2 on PlayStation, ZR on Switch
    // ZR on the Switch is a digital trigger, values will always be either 0 or 255.
    RightTrigger = 1ull << 5,

    // PlayStation only
    TouchpadX = 1ull << 6,

    // PlayStation only
    TouchpadY = 1ull << 7
};

CELL_ENUM_CLASS_OPERATORS(KeyboardButton)
CELL_ENUM_CLASS_OPERATORS(MouseButton)
CELL_ENUM_CLASS_OPERATORS(ControllerButton)
CELL_ENUM_CLASS_OPERATORS(MouseAxis)
CELL_ENUM_CLASS_OPERATORS(ControllerAxis)

}
