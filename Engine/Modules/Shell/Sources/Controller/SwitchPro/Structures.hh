// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Utilities/Preprocessor.hh>

namespace Cell::Shell::Controller {

enum class SwitchProReportId : uint8_t {
    CommandReply = 0x21,
    FullInput = 0x30,
    USBCommandAcknowledge = 0x81
};

enum class SwitchProPacketId : uint8_t {
    RumbleWithCommand = 0x01,
    RumbleOnly = 0x10,
    USBCommand = 0x80
};

enum class SwitchProCommandId : uint8_t {
    RequestDeviceInformation = 0x02,
    SetInputReportMode = 0x03,
    SetPlayerLights = 0x30,
    EnableVibration = 0x48
};

enum class SwitchProUSBCommandId : uint8_t {
    Status = 0x01,
    Handshake = 0x02,
    HighSpeed = 0x03,
    ForceUSB = 0x04,
    ClearUSB = 0x05,
    ResetMCU = 0x06,
};

enum class SwitchProButton : uint32_t {
    A = 1 << 3,
    B = 1 << 2,
    X = 1 << 1,
    Y = 1 << 0,

    Minus = 1 << 8,
    Plus = 1 << 9,

    HOME = 1 << 12,
    Capture = 1 << 13,

    R = 1 << 6,
    ZR = 1 << 7,

    L = 1 << 22,
    ZL = 1 << 23,

    DPadDown = 1 << 16,
    DPadUp = 1 << 17,
    DPadRight = 1 << 18,
    DPadLeft = 1 << 19,

    LeftStick = 1 << 11,
    RightStick = 1 << 10
};

enum class SwitchProState : uint8_t {
    Wired = 0x1
};

enum class SwitchProPlayerIndex : uint8_t {
    None = 0,

    Player1 = 1 << 0,
    Player2 = 1 << 1,
    Player3 = 1 << 2,
    Player4 = 1 << 3
};

struct CELL_PACKED(1) SwitchProCommandPacket {
    SwitchProPacketId packetId;

    uint8_t number;
    uint32_t rumbleData[2];
    SwitchProCommandId command;
    uint8_t data[53];
};

struct CELL_PACKED(1) SwitchProUSBCommandPacket {
    SwitchProPacketId packetId;
    SwitchProUSBCommandId command;

    uint8_t data[62];
};

struct CELL_PACKED(1) SwitchProFullInputReport {
    SwitchProReportId reportId;
    uint8_t counter;
    SwitchProState state;
    uint8_t buttons[3];
    uint8_t leftStick[3];
    uint8_t rightStick[3];
    uint8_t vibrationData;

    struct {
        int16_t accelerometerX;
        int16_t accelerometerY;
        int16_t accelerometerZ;

        int16_t gyroscopeX;
        int16_t gyroscopeY;
        int16_t gyroscopeZ;
    } imuState[3];

    uint8_t padding[15];
};

struct CELL_PACKED(1) SwitchProCommandReplyReport {
    SwitchProReportId reportId;

    uint8_t counter;
    uint8_t state;
    uint8_t buttons[3];
    uint8_t leftStick[3];
    uint8_t rightStick[3];
    uint8_t vibrationData;

    uint8_t isAcked;
    SwitchProCommandId commandId;

    union {
        uint8_t rawData[35];

        /*struct {
            uint32_t address;
            uint8_t length;
            uint8_t data[30];
        } dataReadSPI;*/

        struct {
            uint16_t firmwareVersion;
            uint8_t deviceType;
            uint8_t padding1;
            uint8_t macAddress[6];
            uint8_t padding2;
            uint8_t colorLocation;
        } deviceInfo;

        /*struct {
            uint32_t address;
            uint8_t length;
            uint8_t leftCalibration[9];
            uint8_t rightCalibration[9];
        } stickFactoryCalibration;

        struct {
            uint32_t address;
            uint8_t length;
            uint8_t leftMagic[2];
            uint8_t leftCalibration[9];
            uint8_t rightMagic[2];
            uint8_t rightCalibration[9];
        } stickUserCalibration;*/
    };

    uint8_t padding[13];
};

CELL_ENUM_CLASS_OPERATORS(SwitchProButton);
CELL_ENUM_CLASS_OPERATORS(SwitchProState);
CELL_ENUM_CLASS_OPERATORS(SwitchProPlayerIndex);

#define SWITCH_BUTTONS_U32(report) ((SwitchProButton)((report.buttons[2] << 16 | report.buttons[1] << 8 | report.buttons[0]) ^ (1 << 15)))
#define SWITCH_STATE_BATTERY(state) ((((uint8_t)(state)) & 0xe0) >> 4)

}
