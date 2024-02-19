// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::D3D12 {

// Describes various results of functions interacting with the DirectX 12 implementation.
enum class Result : uint8_t {
    // The operation succeeded.
    Success,

    // The underlying hardware, driver or other software did not offer support for the operation.
    Unsupported,

    // The system ran out of usable memory.
    NotEnoughMemory
};

}
