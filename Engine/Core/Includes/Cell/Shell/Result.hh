// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::Shell {

// Various shell related status codes.
enum class Result : uint8_t {
    // The operation completed successfully.
    Success,

    // The given parameters were malformed.
    InvalidParameters,

    // The platform given was invalid.
    InvalidPlatform,

    // The platform given was not yet ready.
    PlatformNotReadyYet
};

}
