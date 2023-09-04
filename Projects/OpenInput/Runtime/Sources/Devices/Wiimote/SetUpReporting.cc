// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Wiimote.hh"
#include "Communications/ClassicController.hh"
#include "Communications/Nunchuck.hh"
#include "Communications/Requests.hh"
#include <Cell/System/BlockImpl.hh>

using namespace Cell;

namespace Runtime::Devices {
using namespace WiimoteCommunications;

Result Wiimote::SetUpReporting() {
    IO::Result ioResult;
    if ((this->currentStatus.state & StatusState::ExtensionConnected) == StatusState::ExtensionConnected) {
        const WriteRequest setupRequest1(0xa400f0, 0x55);
        ioResult = this->device->Write(System::UnmanagedBlock {&setupRequest1}, 100);
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

        Result result = this->ReceiveAcknowledgement();
        if (result != Result::Success) {
            return result;
        }

        const WriteRequest setupRequest2(0xa400fb, 0x00);
        ioResult = this->device->Write(System::UnmanagedBlock {&setupRequest2}, 100);
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

        result = this->ReceiveAcknowledgement();
        if (result != Result::Success) {
            return result;
        }

        const ReadRequest extensionTypeRequest(0xa400fa, 6);
        ioResult = this->device->Write(System::UnmanagedBlock {&extensionTypeRequest}, 100);
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

        ReadReport readReport {};
        System::UnmanagedBlock readBlock {&readReport};
        ioResult = this->device->Read(readBlock, 100);
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

        if (readReport.reportId != 0x21) {
            return Result::InvalidData;
        }

        if ((readReport.sizeAndError & 0x0f) != 0) {
            return Result::Failure;
        }

        if ((readReport.sizeAndError >> 4) + 1 != 6) {
            return Result::InvalidData;
        }

        if (System::CompareMemory(readReport.data, NunchuckIdentification, 6)) {
            this->extensionType = WiimoteExtensionType::Nunchuck;
        } else if (System::CompareMemory(readReport.data, ClassicControllerIdentification, 6)) {
            this->extensionType = WiimoteExtensionType::ClassicController;
        } else if (System::CompareMemory(readReport.data, ClassicControllerProIdentification, 6)) {
            this->extensionType = WiimoteExtensionType::ClassicControllerPro;
        } else {
            this->extensionType = WiimoteExtensionType::Unknown;
        }
    } else {
        this->extensionType = WiimoteExtensionType::None;
    }

    if (this->extensionType == WiimoteExtensionType::ClassicController || this->extensionType == WiimoteExtensionType::ClassicControllerPro) {
        const WriteRequest ccFormat(0xa400fe, 0x03);
        ioResult = this->device->Write(System::UnmanagedBlock {&ccFormat}, 100);
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

        const Result result = this->ReceiveAcknowledgement();
        if (result != Result::Success) {
            return result;
        }
    }

    const DataReportRequest dataRequest;
    ioResult = this->device->Write(System::UnmanagedBlock {&dataRequest}, 100);
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
