// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/HID.hh>
#include <Cell/System/Panic.hh>
#include <Cell/System/Platform/Windows/Includes.h>

#include <hidsdi.h>

// TODO: Win32 HID doesn't seem to allow non overlapped HID when enabled, which we always have
//       consider whether we should support ms = 0

namespace Cell::IO::HID {

Device::~Device() {
    CloseHandle((HANDLE)this->handle);
}

CELL_FUNCTION_INTERNAL Result handleOverlappedHID(HANDLE handle, OVERLAPPED& overlapped, const DWORD size, const uint32_t milliseconds) {
    const DWORD waitResult = WaitForSingleObject(overlapped.hEvent, milliseconds);
    switch (waitResult) {
    case WAIT_OBJECT_0: {
        break;
    }

    case WAIT_TIMEOUT: {                 // BUG: some Bluetooth devices that disconnect don't always report that / Windows doesn't respect it
        CancelIoEx(handle, &overlapped); //      Instead they keep timing us out, so count this and disconnect automatically
        CloseHandle(overlapped.hEvent);
        return Result::Timeout;
    }

    case WAIT_FAILED: {
        CloseHandle(overlapped.hEvent);
        return Result::Disconnected;
    }

    default: {
        System::Panic("WaitForSingleObject failed");
    }
    }

    DWORD transferred = 0;
    const BOOL result = GetOverlappedResult(handle, &overlapped, &transferred, FALSE);
    if (result == FALSE) {
        switch (GetLastError()) {
        case ERROR_OPERATION_ABORTED: {
            CloseHandle(overlapped.hEvent);
            return Result::Disconnected;
        }

        default: {
            System::Panic("GetOverlappedResult failed");
        }
        }
    }

    CloseHandle(overlapped.hEvent);

    if (transferred < size) {
        return Result::Incomplete;
    }

    return Result::Success;
}

Result Device::Read(IBlock& data, const uint32_t milliseconds) {
    if (data.ByteSize() > UINT32_MAX) {
        return Result::InvalidParameters;
    }

    DWORD read = 0;
    OVERLAPPED overlapped = { .hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr) };

    const BOOL result = ReadFile((HANDLE)this->handle, data.Pointer(), (DWORD)data.ByteSize(), &read, milliseconds > 0 ? &overlapped : nullptr);
    if (result == FALSE) {
        switch (GetLastError()) {
        case ERROR_IO_PENDING: {
            return handleOverlappedHID((HANDLE)this->handle, overlapped, (const DWORD)data.ByteSize(), milliseconds);
        }

        case ERROR_INVALID_PARAMETER:
        case ERROR_INVALID_USER_BUFFER: {
            CloseHandle(overlapped.hEvent);
            return Result::InvalidParameters;
        }

        case ERROR_OPERATION_ABORTED:
        case ERROR_DEVICE_NOT_CONNECTED: {
            CloseHandle(overlapped.hEvent);
            return Result::Disconnected;
        }

        default: {
            System::Panic("ReadFile failed");
        }
        }
    }

    CloseHandle(overlapped.hEvent);

    if (read < data.ByteSize()) {
        return Result::Incomplete;
    }

    return Result::Success;
}

Result Device::Write(const IBlock& data, const uint32_t milliseconds) {
    if (data.ByteSize() > UINT32_MAX) {
        return Result::InvalidParameters;
    }

    DWORD written = 0;
    OVERLAPPED overlapped = { .hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr) };

    const BOOL result = WriteFile((HANDLE)this->handle, data.Pointer(), (DWORD)data.ByteSize(), &written, milliseconds > 0 ? &overlapped : nullptr);
    if (result == FALSE) {
        switch (GetLastError()) {
        case ERROR_IO_PENDING: {
            return handleOverlappedHID((HANDLE)this->handle, overlapped, (const DWORD)data.ByteSize(), milliseconds);
        }

        case ERROR_INVALID_USER_BUFFER:
        case ERROR_INVALID_PARAMETER: {
            CloseHandle(overlapped.hEvent);
            return Result::InvalidParameters;
        }

        case ERROR_DEVICE_NOT_CONNECTED: {
            CloseHandle(overlapped.hEvent);
            return Result::Disconnected;
        }

        default: {
            System::Panic("WriteFile failed");
        }
        }
    }

    CloseHandle(overlapped.hEvent);

    if (written < data.ByteSize()) {
        return Result::Incomplete;
    }

    return Result::Success;
}

Wrapped<Capabilities, Result> Device::GetCapabilities() {
    PHIDP_PREPARSED_DATA data;
    BOOLEAN result = HidD_GetPreparsedData((HANDLE)this->handle, &data);
    if (result == FALSE) {
        System::Panic("HidD_GetPreparsedData failed");
    }

    HIDP_CAPS caps;
    const NTSTATUS status = HidP_GetCaps(data, &caps);
    if (status != HIDP_STATUS_SUCCESS) {
        System::Panic("HidP_GetCaps failed");
    }

    result = HidD_FreePreparsedData(data);
    if (result == FALSE) {
        System::Panic("HidD_FreePreparsedData failed");
    }

    return Capabilities { .InputReportSize = caps.InputReportByteLength, .OutputReportSize = caps.OutputReportByteLength, .FeatureReportSize = caps.FeatureReportByteLength };
}

}
