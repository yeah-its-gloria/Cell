// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Internal.hh"

#include <Cell/IO/USB.hh>
#include <Cell/Memory/Allocator.hh>
#include <Cell/System/Panic.hh>

namespace Cell::IO {

USB::~USB() {
    usbData* data = (usbData*)this->impl;

    WinUsb_Free(data->handle);
    CloseHandle(data->device);

    Memory::Free(data);
}

CELL_FUNCTION_INTERNAL Result handleOverlapped(HANDLE handle, OVERLAPPED& overlapped, const DWORD size, const uint32_t milliseconds) {
    DWORD transferred = 0;
    const BOOL result = GetOverlappedResultEx(handle, &overlapped, &transferred, milliseconds, FALSE);
    if (result == false) {
        switch (GetLastError()) {
        case WAIT_TIMEOUT: {
            return Result::Timeout;
        }

        default: {
            System::Panic("GetOverlappedResultEx failed");
        }
        }
    }

    if (transferred < size) {
        return Result::Incomplete;
    }

    return Result::Success;
}

Result USB::Read(Memory::IBlock& data, const uint8_t endpoint, const uint32_t milliseconds) {
    if (data.GetSize() > UINT32_MAX) {
        return Result::InvalidParameters;
    }

    usbData* interface = (usbData*)this->impl;

    DWORD read = 0;
    OVERLAPPED overlapped;
    const BOOL result = WinUsb_ReadPipe(interface->handle, endpoint, (uint8_t*)data.AsPointer(), (DWORD)data.GetSize(), &read, milliseconds > 0 ? &overlapped : nullptr);
    if (result == FALSE) {
        switch (GetLastError()) {
        case ERROR_IO_PENDING: {
            return handleOverlapped(interface->device, overlapped, (const DWORD)data.GetSize(), milliseconds);
        }

        case ERROR_DEVICE_NOT_CONNECTED: {
            return Result::Disconnected;
        }

        default: {
            System::Panic("WinUsb_ReadPipe failed");
        }
        }
    }

    if (read < data.GetSize()) {
        return Result::Incomplete;
    }

    return Result::Success;
}

Result USB::Write(const Memory::IBlock& data, const uint8_t endpoint, const uint32_t milliseconds) {
    if (data.GetSize() > UINT32_MAX) {
        return Result::InvalidParameters;
    }

    usbData* interface = (usbData*)this->impl;

    DWORD written = 0;
    OVERLAPPED overlapped;
    DWORD stuff = data.GetSize();
    const BOOL result = WinUsb_WritePipe(interface->handle, endpoint, (uint8_t*)data.AsPointer(), (DWORD)stuff, &written, milliseconds > 0 ? &overlapped : nullptr);
    if (result == FALSE) {
        switch (GetLastError()) {
        case ERROR_IO_PENDING: {
            return handleOverlapped(interface->handle, overlapped, (const DWORD)data.GetSize(), milliseconds);
        }

        case ERROR_DEVICE_NOT_CONNECTED: {
            return Result::Disconnected;
        }

        default: {
            System::Panic("WinUsb_WritePipe failed");
        }
        }
    }

    if (written < data.GetSize()) {
        return Result::Incomplete;
    }

    return Result::Success;
}

}
