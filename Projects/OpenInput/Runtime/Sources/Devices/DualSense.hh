// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "Result.hh"
#include <Cell/IO/HID.hh>

namespace Runtime::Devices {

class DualSense : public Cell::Object {
public:
    // Tries to find an uninitialized DualSense controller and initializes it.
    CELL_FUNCTION_INTERNAL static Cell::Wrapped<DualSense*, Result> Find();

    // Destructs the DualSense instance.
    CELL_FUNCTION_INTERNAL ~DualSense();

    // Polls the DualSense controller.
    CELL_FUNCTION_INTERNAL Result Poll();

private:
    CELL_INLINE DualSense(Cell::IO::HID* device) : device(device) {}

    Cell::IO::HID* device;
};

}
