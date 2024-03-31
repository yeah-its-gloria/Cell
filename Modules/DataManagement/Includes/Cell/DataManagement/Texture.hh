// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/DataManagement/Result.hh>
#include <Cell/Memory/Block.hh>

namespace Cell::DataManagement {

// Represents a 1D/2D/3D texture.
class Texture : public Object {
public:
    // Decodes a texture from PNG encoded data.
    CELL_FUNCTION static Wrapped<Texture*, Result> FromPNG(const Memory::IBlock& block);

    // Cleans up the texture contained and destructs the instance.
    CELL_FUNCTION ~Texture();

    CELL_FUNCTION uint32_t* GetBytes() const {
        return this->rgba;
    }

private:
    CELL_FUNCTION_INTERNAL Texture(const uint32_t w, const uint32_t h, const uint8_t d, uint32_t* b) : width(w), height(h), depth(d), rgba(b) { }

    uint32_t width;
    uint32_t height;
    uint8_t depth;

    // RGBA32, little endian
    uint32_t* rgba;
};

}
