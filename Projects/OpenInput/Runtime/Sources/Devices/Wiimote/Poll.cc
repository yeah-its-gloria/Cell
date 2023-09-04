// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Wiimote.hh"
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Log.hh>

using namespace Cell;

namespace Runtime::Devices {
using namespace WiimoteCommunications;

Result Wiimote::Poll() {
    BaseReport report {};
    System::UnmanagedBlock block {&report};

    IO::Result ioResult;
    ioResult = this->device->Read(block, 33);
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

    switch (report.reportId) {
    case 0x20: {
        System::Log("received new status");
        this->currentStatus = *(StatusReport*)&report;
        return this->SetUpReporting();
    }

    case 0x35: {
        this->currentInputData = *(ButtonsAccelExtensionReport*)&report;
        break;
    }

    default: {
        return Result::InvalidData;
    }
    }

    return Result::Success;
}

}
