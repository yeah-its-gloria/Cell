// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Vulkan {

// Various Vulkan related status codes.
enum class Result : uint8_t {
    // The operation completed successfully.
    Success,

    // The swapchain is suboptimal and should be recreated when possible.
    Suboptimal,

    // The instance was not in the correct state for the requested operation.
    InvalidState,

    // The given platform was malformed.
    InvalidPlatform,

    // The given parameters were malformed.
    InvalidParameters,

    // A command or its parameters in the given command list was malformed.
    InvalidCommandGiven,

    // The underlying platform is not yet ready.
    PlatformNotReadyYet,

    // A required feature is missing.
    FeatureMissing,

    // A debug required feature is missing.
    DebugFeatureMissing,

    // The underlying hardware is not compatible.
    RunningOnIncompatibleHardware,

    // The system ran out of memory on the host.
    OutOfHostMemory,

    // The system ran out of memory on the device.
    OutOfDeviceMemory,

    // The device was lost.
    DeviceLost,

    // The surface was lost.
    SurfaceLost,

    // The instance/runtime encountered an error.
    InstanceFailure
};

}
