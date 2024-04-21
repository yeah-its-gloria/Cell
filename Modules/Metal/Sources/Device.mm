// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Metal/Device.hh>

#include <CoreGraphics/CoreGraphics.h>

namespace Cell::Metal {

Wrapped<Device*, Result> Device::New(Shell::Implementations::macOS* shell) {
    CGDirectDisplayID viewDisplayID = (CGDirectDisplayID)[shell->GetWindow().screen.deviceDescription[@"NSScreenNumber"] unsignedIntegerValue];

    id<MTLDevice> device = CGDirectDisplayCopyCurrentMetalDevice(viewDisplayID);
    CELL_ASSERT(device != nullptr);

    return new Device(device);
}

Device::~Device() {
    [this->device release];
}

}
