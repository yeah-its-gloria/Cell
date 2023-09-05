// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Wiimote.hh"
#include "Communications/Requests.hh"
#include <Cell/System/BlockImpl.hh>

using namespace Cell;

namespace Runtime::Devices {
using namespace WiimoteCommunications;

Wrapped<Wiimote*, Result> Wiimote::Find() {
    const Wrapped<IO::HID*, IO::Result> hidResult = IO::HID::Open(0x057e, 0x0306);
    if (!hidResult) {
        switch (hidResult.Result()) {
        case IO::Result::NotFound: {
            return Result::NotFound;
        }

        case IO::Result::Timeout: {
            return Result::Timeout;
        }

        default: {
            System::Panic("Cell::IO::HID::Open failed");
        }
        }
    }

    IO::HID* device = hidResult.Unwrap();
    const StatusRequest statusRequest;
    IO::Result ioResult = device->Write(System::UnmanagedBlock {&statusRequest}, 500);
    switch (ioResult) {
    case IO::Result::Success: {
        break;
    }

    case IO::Result::Timeout: {
        return Result::Timeout;
    }

    case IO::Result::Disconnected: {
        return Result::Disconnected;
    }

    default: {
        System::Panic("Cell:IO::HID::Write failed");
    }
    }

    StatusReport status {};
    System::UnmanagedBlock statusReportBlock {&status};
    ioResult = device->Read(statusReportBlock, 500);
    switch (ioResult) {
    case IO::Result::Success: {
        break;
    }

    case IO::Result::Disconnected: {
        return Result::Disconnected;
    }

    case IO::Result::Timeout: {
        return Result::Timeout;
    }

    default: {
        System::Panic("Cell:IO::HID::Read failed");
    }
    }

    if (status.reportId != 0x20) {
        return Result::InvalidData;
    }

    Wiimote* wiimote = new Wiimote(device, status);
    const Result result = wiimote->SetUpReporting();
    if (result == Result::Success) {
        return wiimote;
    }

    delete wiimote;
    return result;
}

}
