// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Cell.hh>

namespace Cell::DataManagement {

// Represents the types of data stored within a Cell container.
enum class ContainerContentKind : uint8_t {
    // Represents unmanaged, raw data.
    RawData,

    // Represents a Cell archive.
    Archive
};

// Represents the header of a Cell container.
struct CELL_PACKED(1) ContainerHeader {
    // The magic; CELL in UTF-8.
    uint8_t magic[4];

    // The version of the container.
    uint8_t version;

    // The kind of data stored in the container.
    ContainerContentKind kind;
};

// Valid magic for a Cell container.
const uint8_t ContainerMagic[4] = { 'C', 'E', 'L', 'L' };

// Represents the currently highest supported Cell container version.
const uint8_t ContainerVersion = 1;

}
