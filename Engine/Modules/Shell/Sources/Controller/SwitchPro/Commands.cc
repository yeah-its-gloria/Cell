// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Structures.hh"

#include <Cell/Shell/Controller/SwitchPro.hh>
#include <Cell/System/BlockImpl.hh>

namespace Cell::Shell::Controller {

Result SwitchPro::SubmitCommand(uint8_t cmd, uint8_t data) {
    SwitchProCommandPacket command = {
        .packetId = SwitchProPacketId::RumbleWithCommand,

        .number = this->commandCounter,
        .rumbleData = { 0, 0 },
        .command = (SwitchProCommandId)cmd,
        .data = { data }
    };

    IO::Result result = this->device->Write(System::UnownedBlock { &command }, 33);
    switch (result) {
    case IO::Result::Success: {
        break;
    }

    case IO::Result::Timeout: {
        return Result::Timeout;
    }

    default: {
        System::Panic("Cell::IO::HID::Device::Write failed");
    }
    }

    SwitchProCommandReplyReport report = { };
    System::UnownedBlock reportRef { &report };

    uint8_t counter = 0;
    while (counter < 3) {
        result = this->device->Read(reportRef, 33);
        switch (result) {
        case IO::Result::Success: {
            break;
        }

        case IO::Result::Timeout: {
            counter++;
            continue;
        }

        default: {
            System::Panic("Cell::IO::HID::Device::Read failed");
        }
        }

        if (report.reportId != SwitchProReportId::CommandReply) {
            counter++;
            continue;
        }

        CELL_ASSERT(report.commandId == command.command);
        break;
    }

    if (result == IO::Result::Timeout) {
        return Result::Timeout;
    }

    if (counter > 2) {
        return Result::InvalidReplies;
    }

    this->commandCounter = (this->commandCounter + 1) & 0xf;
    return Result::Success;
}

Result SwitchPro::SubmitUSB(uint8_t cmd, const bool waitForReply) {
    SwitchProUSBCommandPacket command = {
        .packetId = SwitchProPacketId::USBCommand,
        .command = (SwitchProUSBCommandId)cmd,
        .data = { 0 }
    };

    IO::Result result = device->Write(System::UnownedBlock { &command }, 33);
    switch (result) {
    case IO::Result::Success: {
        break;
    }

    default: {
        System::Panic("Cell::IO::HID::Device::Write failed");
    }
    }

    if (waitForReply) {
        System::OwnedBlock<uint8_t> reply(64);
        uint8_t counter = 0;
        while (counter < 3) {
            result = device->Read(reply, 1000);
            switch (result) {
            case IO::Result::Success: {
                break;
            }

            default: {
                System::Panic("Cell::IO::HID::Device::Read failed");
            }
            }

            if ((SwitchProReportId)reply[0] != SwitchProReportId::USBCommandAcknowledge) {
                counter++;
                continue;
            }

            CELL_ASSERT(reply[1] == (uint8_t)command.command);
            break;
        }

        if (counter > 2) {
            return Result::InvalidReplies;
        }
    }

    if (result == IO::Result::Timeout) {
        return Result::Timeout;
    }

    return Result::Success;
}

}
