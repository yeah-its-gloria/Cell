#pragma once

#include <Cell/Cell.hh>
#include <openinput/openinput.h>

namespace Runtime {

enum class MessageType : uint16_t {
    Invalid,
    Connect,
    CreateInstance,
    GetDeviceCount,
    ResponseGetDeviceCount,
    GetDeviceInfos,
    ResponseGetDeviceInfos
};

struct CELL_PACKED(1) Message {
    MessageType messageType;
    uint8_t data[512];
};

struct CELL_PACKED(1) MessageConnect {
    MessageType messageType;
    uint64_t runtimeVersion;
    uint8_t padding[512 - sizeof(uint64_t)];
};

struct CELL_PACKED(1) MessageCreateInstance {
    MessageType messageType;
    OiInstanceCreateInfo info;
    uint8_t padding[512 - sizeof(OiInstanceCreateInfo)];
};

struct CELL_PACKED(1) MessageResponseGetDeviceCount {
    MessageType messageType;
    uint32_t count;
    uint8_t padding[512 - sizeof(uint32_t)];
};

struct CELL_PACKED(1) MessageResponseDeviceInfos {
    MessageType messageType;
    uint32_t index;
    OiDeviceInfo info;
    uint8_t padding[512 - sizeof(uint32_t) - sizeof(OiDeviceInfo)];
};

}
