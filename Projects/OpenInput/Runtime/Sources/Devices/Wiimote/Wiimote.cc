// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Wiimote.hh"

#include "Communications/Requests.hh"
#include <Cell/System/BlockImpl.hh>

using namespace Cell;

namespace Runtime::Devices {
using namespace WiimoteCommunications;

Wiimote::~Wiimote() {
    delete this->device;
}

Result Wiimote::ReceiveAcknowledgement() {
    AcknowledgeReport report {};
    System::UnmanagedBlock block {&report};

    IO::Result ioResult = this->device->Read(block, 33);
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

    if (report.reportId != 0x22) {
        return Result::InvalidData;
    }

    switch (report.acknowledgedReportId) {
    case 0x16:
    case 0x17: {
        break;
    }

    default: {
        return Result::InvalidData;
    }
    }

    if (report.result != 0) {
        return Result::Failure;
    }

    return Result::Success;
}

Result Wiimote::SetLEDs(LEDMask mask) {
    const LEDsRequest ledRequest(mask);
    IO::Result ioResult = this->device->Write(System::UnmanagedBlock {&ledRequest}, 500);
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
        System::Panic("Cell:IO::HID::Write failed");
    }
    }

    return Result::Success;
}

Result Wiimote::SetRumble(const bool enable) {
    const RumbleRequest rumbleRequest(enable);
    IO::Result ioResult = this->device->Write(System::UnmanagedBlock {&rumbleRequest}, 500);
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
        System::Panic("Cell:IO::HID::Write failed");
    }
    }

    return Result::Success;
}

}
