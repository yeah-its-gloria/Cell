// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/DataManagement/Result.hh>

namespace Cell::DataManagement::Foreign {

// Manages PNG data.
class PNG : public Object {
public:
    // Decodes PNG data into a new instance.
    CELL_FUNCTION static Wrapped<PNG*, Result> Decode(const uint8_t* CELL_NONNULL data, const size_t size);

    // Destructs this PNG instance.
    CELL_FUNCTION ~PNG();

private:
    CELL_INLINE PNG(uint32_t width, uint32_t height, uint8_t depth, uint8_t* data) : width(width), height(height), depth(depth), data(data) { }

    uint32_t width;
    uint32_t height;
    uint8_t depth;

    uint8_t* data;
};

}
