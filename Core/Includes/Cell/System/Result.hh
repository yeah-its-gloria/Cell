// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::System {

// Various system relevant status codes.
enum class Result : uint8_t {
    // The operation completed successfully.
    Success,

    // The operation timed out.
    Timeout,

    // The given element is empty.
    IsEmpty,

    // The given parameters were malformed.
    InvalidParameters,

    // The given element (e.g a thread) died.
    ElementHasDied,

    // Failed to convert an element from one format to another.
    ConversionFailure,

    // The requested element was not found.
    NotFound,

    // The subsystem lacked enough memory.
    OutOfMemory
};

}
