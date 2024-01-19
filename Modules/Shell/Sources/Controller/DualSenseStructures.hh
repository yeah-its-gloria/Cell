// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Utilities/Preprocessor.hh>

namespace Cell::Shell::Controller {

enum class DualSenseButton : uint32_t {
    DPadMask = 0x0f,

    Square = 1 << 4,
    Cross = 1 << 5,
    Circle = 1 << 6,
    Triangle = 1 << 7,
    L1 = 1 << 8,
    R1 = 1 << 9,
    L2 = 1 << 10,
    R2 = 1 << 11,
    Create = 1 << 12,
    Options = 1 << 13,
    L3 = 1 << 14,
    R3 = 1 << 15,
    PS = 1 << 16,
    Touchpad = 1 << 17
};

enum class DualSenseEffects : uint16_t {
    None = 0,

    // Requires audio haptics to be disabled to function properly.
    EnableRumble = 1 << 0,

    NoAudioHaptics = 1 << 1,

    SetMuteLED = 1 << 8,
    SetLEDs = 1 << 10,
    ResetLEDs = 1 << 11,
    SetPlayerLEDs = 1 << 12
};

enum class DualSenseEffectsEx : uint8_t {
    None = 0,

    // Requires audio haptics to be disabled to function properly.
    // Requires firmware >= 2.24.
    ImprovedRumble = 1 << 2
};

enum class DualSenseMuteLightMode : uint8_t {
    Off   = 0x00,
    Solid = 0x01,
    Pulse = 0x02
};

enum class DualSensePlayerLEDs : uint8_t {
    None = 0,

    First = 1 << 0,
    Two = 1 << 1,
    Three = 1 << 2,
    Four = 1 << 3,
    Five = 1 << 4,

    Player1 = Three,
    Player2 = Two | Four,
    Player3 = First | Three | Five,
    Player4 = First | Two | Four | Five,
    Player5 = First | Two | Three | Four | Five,

    // Instantly switch LEDs instead of fading.
    Instant = 1 << 5
};

struct CELL_PACKED(1) DualSenseReportPacket {
    uint8_t id;
    uint8_t leftStickX;
    uint8_t leftStickY;
    uint8_t rightStickX;
    uint8_t rightStickY;
    uint8_t leftTrigger;
    uint8_t rightTrigger;
    uint8_t counter;
    DualSenseButton buttons;
    uint32_t sequence;
    uint16_t gyroscope[3];
    uint16_t accelerometer[3];
    uint32_t sensorTimestamp;
    uint8_t sensorTemperature;
    uint8_t touchpadCounter1;
    uint8_t touchpadData1[3];
    uint8_t touchpadCounter2;
    uint8_t touchpadData2[3];
    uint64_t padding;
    uint32_t timer;
    uint8_t battery;
    uint8_t state;
    uint8_t padding2[9];
};

struct CELL_PACKED(1) DualSenseEffectsPacket {
    uint8_t id;
    DualSenseEffects effectToggles;
    uint8_t rumbleRight;
    uint8_t rumbleLeft;
    uint8_t headphoneVolume;
    uint8_t speakerVolume;
    uint8_t microphoneVolume;
    uint8_t audioToggles;
    DualSenseMuteLightMode muteLightMode;
    uint8_t audioMuteToggles;
    uint8_t rightTriggerEffectData[11];
    uint8_t leftTriggerEffectData[11];
    uint8_t unknown1[6];
    DualSenseEffectsEx effectTogglesEx;
    uint16_t unknown2;
    uint8_t ledAnimation;
    uint8_t ledBrightness;
    DualSensePlayerLEDs playerLEDs;
    uint8_t ledRed;
    uint8_t ledGreen;
    uint8_t ledBlue;
};

CELL_ENUM_CLASS_OPERATORS(DualSenseButton);
CELL_ENUM_CLASS_OPERATORS(DualSenseEffects);
CELL_ENUM_CLASS_OPERATORS(DualSenseEffectsEx);
CELL_ENUM_CLASS_OPERATORS(DualSensePlayerLEDs);

}
