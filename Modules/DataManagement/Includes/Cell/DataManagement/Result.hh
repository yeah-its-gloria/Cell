// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::DataManagement {

// Results used across data parsing.
enum class Result : uint8_t {
    // Success.
    Success,

    // Malformed parameters.
    InvalidParameters,

    // Indicates a broken file signature.
    InvalidSignature,

    // Unexpected or unsupported size.
    InvalidSize,

    // Malformed identifier, for something like a chunk.
    InvalidIdentifier,

    // Checksum was wrong.
    InvalidChecksum,

    // Malformed data.
    InvalidData,

    // Buffer too small.
    NoSpaceInBuffer,

    // Ran out of memory.
    NotEnoughMemory
};

}
