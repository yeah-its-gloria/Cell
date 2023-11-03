// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Shell {

// Various shell related status codes.
enum class Result : uint8_t {
    // The operation completed successfully.
    Success,

    // There were no updates to process.
    NoUpdates,

    // The given parameters were malformed.
    InvalidParameters,

    // The shell informed us that the user wants to quit.
    RequestedQuit,

    // The controller wasn't found.
    NotFound,

    // The request timed out.
    Timeout,

    // The device (e.g controller) responded improperly.
    InvalidReplies
};

}
