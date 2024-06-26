// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/String.hh>
#include <Cell/IO/HID.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#include <devpkey.h>
#include <hidsdi.h>
#include <SetupAPI.h>

namespace Cell::IO::HID {

struct deviceProps {
    bool isBluetooth;
    bool isValid;
    uint16_t vendorId;
    uint16_t productId;
};

CELL_FUNCTION_INTERNAL deviceProps parseDevicePathHID(const wchar_t* data) {
    deviceProps props = { false, false, 0, 0 };

    const size_t pidOffset = StringDetails::RawStringSize("_PID&");

    String path = String::FromPlatformWideString(data).Unwrap();
    if (path.BeginsWith("HID\\VID")) { // USB
        const size_t offset = StringDetails::RawStringSize("HID\\VID_");

        props.isValid = true;
        props.vendorId = path.Substring(offset, 4).Unwrap().AsNumber(true).Unwrap();
        props.productId = path.Substring(offset + 4 + pidOffset, 4).Unwrap().AsNumber(true).Unwrap();
    } else if (path.BeginsWith("HID\\{")) { // USB
        props.isBluetooth = true;

        const size_t baseOffset = StringDetails::RawStringSize("HID\\{00000000-0000-0000-0000-000000000000}");
        if (path.Substring(baseOffset, 4).Unwrap() == "_Dev") {
            const size_t offset = baseOffset + StringDetails::RawStringSize("_Dev_VID&00");

            props.vendorId = path.Substring(offset, 4).Unwrap().AsNumber(true).Unwrap();
            props.productId = path.Substring(offset + 4 + pidOffset, 4).Unwrap().AsNumber(true).Unwrap();
        } else {
            const size_t offset = baseOffset + StringDetails::RawStringSize("_VID&0000");

            props.vendorId = path.Substring(offset, 4).Unwrap().AsNumber(true).Unwrap();
            props.productId = path.Substring(offset + 4 + pidOffset, 4).Unwrap().AsNumber(true).Unwrap();
        }

        props.isValid = true;
    }

    return props;
}

Wrapped<Device*, Result> Device::Open(const uint16_t vendorId, const uint16_t productId) {
    GUID hidGUID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
    HidD_GetHidGuid(&hidGUID);

    constexpr GUID deviceClassHID = { 0x745a17a0, 0x74d3, 0x11d0, { 0xb6, 0xfe, 0x00, 0xa0, 0xc9, 0x0f, 0x57, 0xda } };
    HDEVINFO info = SetupDiGetClassDevsW(nullptr, nullptr, nullptr, DIGCF_ALLCLASSES | DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (info == INVALID_HANDLE_VALUE || info == nullptr) {
        System::Panic("SetupDiGetClassDevsW failed");
    }

    SP_DEVINFO_DATA data = {
        .cbSize = sizeof(SP_DEVINFO_DATA),
        .ClassGuid = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } },
        .DevInst = 0,
        .Reserved = 0
    };

    SP_DEVICE_INTERFACE_DATA interfaceData = {
        .cbSize = sizeof(SP_DEVICE_INTERFACE_DATA),
        .InterfaceClassGuid = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } },
        .Flags = 0,
        .Reserved = 0
    };

    SP_DEVICE_INTERFACE_DETAIL_DATA_W* interfaceDetailData;
    DWORD interfaceDetailDataSize = 0;

    bool isBluetooth = false;
    wchar_t* path = nullptr;
    for (uint32_t index = 0; index < UINT32_MAX; index++) {
        BOOL result = SetupDiEnumDeviceInfo(info, index, &data);
        if (result == FALSE) {
            if (GetLastError() == ERROR_NO_MORE_ITEMS) {
                break;
            }

            System::Panic("SetupDiEnumDeviceInfo failed");
        }

        if (!IsEqualGUID(data.ClassGuid, deviceClassHID)) {
            continue;
        }

        DEVPROPTYPE propertyType = 0;
        DWORD size = 0;

        result = SetupDiGetDevicePropertyW(info, &data, &DEVPKEY_Device_HardwareIds, &propertyType, nullptr, 0, &size, 0);
        if (result == FALSE) {
            switch (GetLastError()) {
            case ERROR_INSUFFICIENT_BUFFER: {
                break;
            }

            default: {
                System::Panic("SetupDiGetDevicePropertyW failed");
            }
            }
        }

        CELL_ASSERT(propertyType == DEVPROP_TYPE_STRING_LIST);

        ScopedBlock<wchar_t> hardwareIDs = Memory::Allocate<wchar_t>(size + 1);
        result = SetupDiGetDevicePropertyW(info, &data, &DEVPKEY_Device_HardwareIds, &propertyType, (BYTE*)(wchar_t*)hardwareIDs, size, &size, 0);
        if (result == FALSE) {
            System::Panic("SetupDiGetDevicePropertyW failed");
        }

        const deviceProps props = parseDevicePathHID(hardwareIDs);
        if (!props.isValid || props.vendorId != vendorId || props.productId != productId) {
            continue;
        }

        result = SetupDiEnumDeviceInterfaces(info, &data, &hidGUID, 0, &interfaceData);
        if (result == FALSE) {
            System::Panic("SetupDiEnumDeviceInterfaces failed");
        }

        result = SetupDiGetDeviceInterfaceDetailW(info, &interfaceData, nullptr, 0, &interfaceDetailDataSize, nullptr);
        if (result == FALSE && GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
            System::Panic("SetupDiGetDeviceInterfaceDetailW failed");
        }

        interfaceDetailData = (SP_DEVICE_INTERFACE_DETAIL_DATA_W*)Memory::Allocate<uint8_t>(interfaceDetailDataSize);
        interfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_W);

        result = SetupDiGetDeviceInterfaceDetailW(info, &interfaceData, interfaceDetailData, interfaceDetailDataSize, &interfaceDetailDataSize, nullptr);
        if (result == FALSE) {
            System::Panic("SetupDiGetDeviceInterfaceDetailW failed");
        }

        isBluetooth = props.isBluetooth;
        path = Memory::Allocate<wchar_t>(wcslen(interfaceDetailData->DevicePath) + 1);
        wcscpy(path, interfaceDetailData->DevicePath);

        Memory::Free(interfaceDetailData);
        break;
    }

    SetupDiDestroyDeviceInfoList(info);

    if (path == nullptr) {
        return Result::NotFound;
    }

    HANDLE device = CreateFileW(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
    if (device == INVALID_HANDLE_VALUE || device == nullptr) {
        switch (GetLastError()) {
        case ERROR_SHARING_VIOLATION: {
            return Result::Locked;
        }

        default: {
            System::Panic("CreateFileW failed");
        }
        }
    }

    Memory::Free(path);

    return new Device((uintptr_t)device, isBluetooth ? ConnectionType::Bluetooth : ConnectionType::USB);
}

}
