// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Renderer {

// Various Renderer related status codes.
enum class Result : uint8_t {
    // The operation completed successfully.
    Success,

    // The current resources (e.g a swapchain) turned suboptimal and should be recreated with the current environment.
    Suboptimal,

    // The given parameters were malformed.
    InvalidParameters,

    // A command or its parameters in the given command list was malformed.
    InvalidCommandGiven,

    // A required feature is missing.
    FeatureMissing,

    // A debug required feature is missing.
    DebugFeatureMissing,

    // The underlying interface is not compatible.
    Unsupported,

    // The system ran out of memory on the host.
    //
    // Some interfaces (e.g D3D12) don't fully differentiate between where the resource is stored.
    OutOfHostMemory,

    // The system ran out of memory on the device.
    OutOfDeviceMemory,

    // The device was lost.
    DeviceLost,

    // The surface was lost.
    SurfaceLost,

    // The instance/runtime encountered an error.
    ManagerFailure
};

}
