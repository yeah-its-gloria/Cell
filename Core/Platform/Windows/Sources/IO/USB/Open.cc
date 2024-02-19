// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Internal.hh"

#include <Cell/IO/USB.hh>
#include <Cell/StringDetails/RawString.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Log.hh>

#include <devpkey.h>
#include <SetupAPI.h>

namespace Cell::IO {

struct deviceProps {
    bool isValid;
    uint16_t vendorId;
    uint16_t productId;
    uint8_t interface;
};

CELL_FUNCTION_INTERNAL deviceProps parseDevicePath(const wchar_t* data) {
    deviceProps props = { false, 0, 0, 0 };

    const size_t pidOffset = StringDetails::RawStringSize("_PID&");
    const size_t interfaceOffset = StringDetails::RawStringSize("&REV_0000&MI_");

    String path = String::FromPlatformWideString(data).Unwrap();
    if (path.BeginsWith("USB\\VID")) {
        const size_t offset = StringDetails::RawStringSize("USB\\VID_");

        props.isValid = true;
        props.vendorId = path.Substring(offset, 4).Unwrap().AsNumber(true).Unwrap();
        props.productId = path.Substring(offset + 4 + pidOffset, 4).Unwrap().AsNumber(true).Unwrap();

        if (path.GetSize() >= offset + pidOffset + interfaceOffset + 8 + 2) {
            props.interface = path.Substring(offset + pidOffset + interfaceOffset + 8, 2).Unwrap().AsNumber(true).Unwrap();
        }
    } else if (path.BeginsWith("USB\\{")) {
        const size_t offset = StringDetails::RawStringSize("USB\\{00000000-0000-0000-0000-000000000000}_VID&0000");

        props.isValid = true;
        props.vendorId = path.Substring(offset, 4).Unwrap().AsNumber(true).Unwrap();
        props.productId = path.Substring(offset + 4 + pidOffset, 4).Unwrap().AsNumber(true).Unwrap();

        if (path.GetSize() >= offset + pidOffset + interfaceOffset + 8 + 2) {
            props.interface = path.Substring(offset + pidOffset + interfaceOffset + 8, 2).Unwrap().AsNumber(true).Unwrap();
        }
    }

    return props;
}

Wrapped<USB*, Result> USB::Open(const uint16_t vendorId, const uint16_t productId, const uint8_t interface) {
    constexpr GUID deviceClassWinUSB = { 0x88bae032, 0x5a81, 0x49f0, { 0xbc, 0x3d, 0xa4, 0xff, 0x13, 0x82, 0x16, 0xd6 } };
    constexpr GUID deviceInterfaceWinUSB = {0xa5dcbf10, 0x6530, 0x11d2, { 0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed } };

    HDEVINFO info = SetupDiGetClassDevsW(&deviceInterfaceWinUSB, nullptr, nullptr, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (info == INVALID_HANDLE_VALUE || info == nullptr) {
        System::Panic("SetupDiGetClassDevsW failed");
    }

    SP_DEVINFO_DATA data = {
        .cbSize    = sizeof(SP_DEVINFO_DATA),
        .ClassGuid = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } },
        .DevInst   = 0,
        .Reserved  = 0
    };

    SP_DEVICE_INTERFACE_DATA interfaceData = {
        .cbSize             = sizeof(SP_DEVICE_INTERFACE_DATA),
        .InterfaceClassGuid = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } },
        .Flags              = 0,
        .Reserved           = 0
    };

    SP_DEVICE_INTERFACE_DETAIL_DATA_W* interfaceDetailData;
    DWORD interfaceDetailDataSize = 0;

    wchar_t* path = nullptr;
    for (uint32_t index = 0; index < UINT32_MAX; index++) {
        BOOL result = SetupDiEnumDeviceInfo(info, index, &data);
        if (result == FALSE) {
            if (GetLastError() == ERROR_NO_MORE_ITEMS) {
                break;
            } else {
                System::Panic("SetupDiEnumDeviceInfo failed");
            }
        }

        if (!IsEqualGUID(data.ClassGuid, deviceClassWinUSB)) {
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

        System::OwnedBlock<wchar_t> hardwareIDs(size + 1);
        result = SetupDiGetDevicePropertyW(info, &data, &DEVPKEY_Device_HardwareIds, &propertyType, (BYTE*)hardwareIDs.Pointer(), size, &size, 0);
        if (result == FALSE) {
            System::Panic("SetupDiGetDevicePropertyW failed");
        }

        const deviceProps props = parseDevicePath(hardwareIDs);
        if (!props.isValid || props.vendorId != vendorId || props.productId != productId || props.interface != interface) {
            continue;
        }

        result = SetupDiEnumDeviceInterfaces(info, &data, &deviceInterfaceWinUSB, 0, &interfaceData);
        if (result == FALSE) {
            System::Panic("SetupDiEnumDeviceInterfaces failed");
        }

        result = SetupDiGetDeviceInterfaceDetailW(info, &interfaceData, nullptr, 0, &interfaceDetailDataSize, nullptr);
        if (result == FALSE) {
            if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
                System::Panic("SetupDiGetDeviceInterfaceDetailW failed");
            }
        }

        interfaceDetailData = (SP_DEVICE_INTERFACE_DETAIL_DATA_W*)System::AllocateMemory(interfaceDetailDataSize);
        interfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_W);

        result = SetupDiGetDeviceInterfaceDetailW(info, &interfaceData, interfaceDetailData, interfaceDetailDataSize, &interfaceDetailDataSize, nullptr);
        if (result == FALSE) {
            System::Panic("SetupDiGetDeviceInterfaceDetailW failed");
        }

        path = System::AllocateMemory<wchar_t>(wcslen(interfaceDetailData->DevicePath) + 1);
        wcscpy(path, interfaceDetailData->DevicePath);

        System::FreeMemory(interfaceDetailData);
        break;
    }

    SetupDiDestroyDeviceInfoList(info);

    if (path == nullptr) {
        return Result::NotFound;
    }

    HANDLE _device = CreateFileW(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
    CELL_ASSERT(_device != INVALID_HANDLE_VALUE && _device != nullptr);

    System::FreeMemory(path);

    WINUSB_INTERFACE_HANDLE winUsbDevice;
    const BOOL result = WinUsb_Initialize(_device, &winUsbDevice);
    if (result == FALSE) {
        switch (GetLastError()) {
        case ERROR_NOT_ENOUGH_MEMORY: {
            CloseHandle(_device);
            return Result::NotEnoughMemory;
        }

        case ERROR_BAD_DEVICE: {
            CloseHandle(_device);
            return Result::Broken;
        }

        default: {
            System::Panic("WinUsb_Initialize failed");
        }
        }
    }

    usbData* devData = System::AllocateMemory<usbData>();

    devData->device = _device;
    devData->handle = winUsbDevice;

    return new USB((uintptr_t)devData);
}

}
