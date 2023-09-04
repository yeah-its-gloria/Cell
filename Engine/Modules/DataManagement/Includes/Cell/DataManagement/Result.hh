// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::DataManagement {

// Results used across data parsing.
enum class Result : uint8_t {
    // Indicates success.
    Success,

    // Indicates malformed parameters.
    InvalidParameters,

// PNG

    // Indicates a broken file signature.
    InvalidSignature,

    // Indicates an unexpected or unsupported size.
    InvalidSize,

    // Indicates a malformed identifier, for something like a chunk.
    InvalidIdentifier,

    // Indicates a checksum was wrong.
    InvalidChecksum,

    // Indicates malformed data.
    InvalidData
};

}
