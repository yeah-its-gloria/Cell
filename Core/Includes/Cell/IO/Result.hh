// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::IO {

// Status codes for IO operations.
enum class Result : uint8_t {
    // The operation completed successfully.
    Success,

    // The given action completed, but did not finish entirely; e.g reading a larger number of bytes than the file count itself.
    Incomplete,

    // The list of elements has reached its end.
    NoMoreElements,

    // The given operation cannot be performed.
    InvalidOperation,

    // The given parameters were malformed.
    InvalidParameters,

    // There was not enough storage available.
    InsufficientStorage,

    // The system and/or underlying API ran out of usable memory.
    NotEnoughMemory,

    // The given element could not be found.
    NotFound,

    // The given element already exists.
    AlreadyExists,

    // The given element could not be accessed.
    AccessDenied,

    // The given element is locked and cannot be used at the moment.
    Locked,

    // The cursor for the given element reached the end.
    ReachedEnd,

    // The operation timed out.
    Timeout,

    // The entity (e.g file, device) could not be put into a usable state.
    Broken,

    // The given element disconnected in some fashion.
    Disconnected
};

}
