// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../GameCubeAdapter.hh"

#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Log.hh>

using namespace Cell;

namespace Runtime::Devices {


Cell::Wrapped<GameCubeAdapter*, Result> GameCubeAdapter::Find() {
    const Wrapped<IO::USB*, IO::Result> usbResult = IO::USB::Open(0x057e, 0x0337);
    if (!usbResult) {
        switch (usbResult.Result()) {
        case IO::Result::NotFound: {
            return Result::NotFound;
        }

        case IO::Result::Timeout: {
            return Result::Timeout;
        }

        default: {
            System::Panic("Cell::IO::USB::Open failed");
        }
        }
    }

    Cell::IO::USB* usb = usbResult.Unwrap();

    const char payload = 0x13;
    const IO::Result result = usb->Write(System::UnmanagedBlock { &payload }, 0x02);
    CELL_ASSERT(result == IO::Result::Success);

    return new GameCubeAdapter(usb);
}

GameCubeAdapter::~GameCubeAdapter() {
    delete this->device;
}

Result GameCubeAdapter::Poll() {
    uint8_t payload[37] { 0 };
    System::UnmanagedBlock payloadBlock { payload, 37 };

    const IO::Result result = this->device->Read(payloadBlock, 0x81);
    CELL_ASSERT(result == IO::Result::Success);
    CELL_ASSERT(payload[0] == 0x21);

    System::CopyMemory(this->reports, (GameCubeAdapter*)payload + 1, 4);
    return Result::Success;
}

}