// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Audio {

// Various audio related status codes.
enum class Result : uint8_t {
    // The operation completed successfully.
    Success,

    // The instance is already in the requested state. This does not represent complete failure.
    AlreadyInRequestedState,

    // The given instance was in the wrong state for the requested operation.
    InvalidState,

    // The given parameters were malformed.
    InvalidParameters,

    // The given platform is not yet ready.
    PlatformNotReadyYet,

    // The underlying runtime encountered an error.
    RuntimeFailure,

    // A required feature is not available.
    FeatureMissing,

    // A required debugging feature is not available.
    DebugFeatureMissing,

    // The current hardware is not capable of supporting the requested features.
    RunningOnIncompatibleHardware,

    // The current underlying implementation is not compatible.
    RunningOnIncompatibleSoftware,

    // The requested format isn't supported.
    UnsupportedFormat,

    // The underlying device was lost.
    DeviceLost,

    // The requested device is occupied by another software.
    DeviceOccupied,

    // The underlying subsystem ran out of usable memory.
    OutOfMemory,

    // The subsystem is not yet finished with a previous operation and cannot begin another just yet.
    NotYetFinished
};

}
