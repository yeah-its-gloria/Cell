// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Audio {

// Various audio related status codes.
enum class Result : uint8_t {
    // The operation completed successfully.
    Success,

    // The given parameters were malformed.
    InvalidParameters,

    // The requested format isn't supported.
    UnsupportedFormat,

    // The underlying target (speakers, microphone, etc.)
    TargetLost,

    // The requested target (speakers, microphone, etc.) is currently exclusively used by another application.
    TargetOccupied,

    // The subsystem couldn't be accessed.
    SubsystemBlocked,

    // The operation couldn't acquire enough memory.
    NotEnoughMemory,

    // The necessary underlying services weren't available.
    ServiceUnavailable
};

}
