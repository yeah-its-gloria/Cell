// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::StringDetails {

// Various system relevant status codes.
enum class Result : uint8_t {
    // The operation completed successfully.
    Success,

    // The given content is empty.
    IsEmpty,

    // The given parameters were malformed.
    InvalidParameters,

    // The content was malformed or unrecognizable.
    InvalidFormat,

    // Not enough memory was present.
    NotEnoughMemory
};

}
