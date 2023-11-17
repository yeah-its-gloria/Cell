// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Structures.hh"

#include <Cell/Shell/Controller/SwitchPro.hh>
#include <Cell/System/BlockImpl.hh>

namespace Cell::Shell::Controller {

Result SwitchPro::SubmitCommand(uint8_t cmd, uint8_t data, const bool allowFailure) {
    SwitchProCommandPacket command = {
        .packetId = SwitchProPacketId::RumbleWithCommand,

        .number = this->commandCounter,
        .rumbleData = { 0x40400100, 0x40400100 },
        .command = (SwitchProCommandId)cmd,
        .data = { data }
    };

    IO::Result result = IO::Result::Success;

    if (this->type == IO::HID::ConnectionType::Bluetooth) {
        System::OwnedBlock<uint8_t> commandBlock(49);
        System::CopyMemory<uint8_t>(commandBlock, (uint8_t*)&command, sizeof(SwitchProCommandPacket));

        result = this->device->Write(commandBlock, 33);
    } else {
        result = this->device->Write(System::UnownedBlock { &command }, 33);
    }

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

    const int maxTries = 6;

    uint8_t counter = 0;
    while (counter < maxTries) {
        System::OwnedBlock<uint8_t> reportRef(this->type == IO::HID::ConnectionType::Bluetooth ? 362 : 64);
        result = this->device->Read(reportRef, 66);
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

        SwitchProCommandReplyReport* report = (SwitchProCommandReplyReport*)reportRef.Pointer();
        if (report->reportId != SwitchProReportId::CommandReply) {
            counter++;
            continue;
        }

        if (report->commandId != command.command) {
            return Result::InvalidReplies;
        }

        break;
    }

    if (result == IO::Result::Timeout) {
        return Result::Timeout;
    }

    if (counter >= maxTries && !allowFailure) {
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

    IO::Result result = IO::Result::Success;

    if (this->type == IO::HID::ConnectionType::Bluetooth) {
        System::OwnedBlock<uint8_t> commandBlock(362);
        System::CopyMemory<uint8_t>(commandBlock, (uint8_t*)&command, sizeof(SwitchProUSBCommandPacket));

        result = this->device->Write(commandBlock, 33);
    } else {
        result = this->device->Write(System::UnownedBlock { &command }, 33);
    }

    switch (result) {
    case IO::Result::Success: {
        break;
    }

    default: {
        System::Panic("Cell::IO::HID::Device::Write failed");
    }
    }

    if (waitForReply) {
        System::OwnedBlock<uint8_t> reply(this->type == IO::HID::ConnectionType::Bluetooth ? 49 : 64);
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
