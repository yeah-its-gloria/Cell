// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Internal.hh"

#include <Runtime.hh>

#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Panic.hh>

OiResult oiEnumerateDevices(OiInstance instance, uint32_t* count, OiDevice* devices) {
    if (instance == nullptr || count == nullptr || (*count > 0 && devices == nullptr)) {
        return OI_ERROR_INVALID_PARAMETERS;
    }

    Runtime::Message request { Runtime::MessageType::GetDeviceCount, { 0 } };
    Cell::IO::Result result = instance->pipe->Write(Cell::System::UnmanagedBlock { &request });
    switch (result) {
    case Cell::IO::Result::Success: {
        break;
    }

    default: {
        Cell::System::Panic("Cell::IO::Pipe::Write failed");
    }
    }

    Runtime::MessageResponseGetDeviceCount response { Runtime::MessageType::ResponseGetDeviceCount, 0, { 0 } };
    Cell::System::UnmanagedBlock responseBlock { &response };
    result = instance->pipe->Read(responseBlock);
    switch (result) {
    case Cell::IO::Result::Success: {
        break;
    }

    default: {
        Cell::System::Panic("Cell::IO::Pipe::Read failed");
    }
    }

    CELL_ASSERT(response.messageType == Runtime::MessageType::ResponseGetDeviceCount);

    // TODO: get devices

    *count = response.count;
    return OI_SUCCESS;
}

OiResult oiGetDeviceInfo(OiDevice device, OiDeviceInfo* info) {
    if (device == nullptr || info == nullptr) {
        return OI_ERROR_INVALID_PARAMETERS;
    }

    return OI_SUCCESS;
}
