// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::OpenXR {

// Various XR related status codes.
enum class Result : uint8_t {
    // The operation completed successfully.
    Success,

    // The session is already active. This does not strictly represent failure.
    AlreadyActive,

    // The session/frame is not yet ready. This does not strictly represent failure.
    NotYetReady,

    // A view is malformed; e.g an invalid pose. This does not strictly represent failure.
    ViewOrPoseMalformed,

    // The session is not currently active. This does not strictly represent failure.
    NotActive,

    // The instance was not in the correct state for the given operation.
    InvalidState,

    // The renderer was not in the correct state for the given operation.
    InvalidRendererState,

    // The given parameters were malformed.
    InvalidParameters,

    // A component is not yet available.
    Unavailable,

    // The runtime is not yet available or active.
    RuntimeUnavailable,

    // The runtime encountered an error.
    RuntimeFailure,

    // The runtime lost its connection.
    RuntimeLost,

    // The session was lost.
    SessionLost,

    // A required feature is not available.
    FeatureMissing,

    // A required debug feature is not available.
    DebugFeatureMissing,

    // The runtime ran out of memory.
    OutOfMemory,

    // The number of relevant elements (e.g sessions, spaces) supported by the runtime was exceeded.
    LimitExceeded,

    // The underlying hardware is not compatible.
    UnsupportedHardware
};

}
