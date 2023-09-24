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

    // The platform given was invalid.
    InvalidPlatform,

    // The shell informed us that the user wants to quit.
    RequestedQuit
};

}
