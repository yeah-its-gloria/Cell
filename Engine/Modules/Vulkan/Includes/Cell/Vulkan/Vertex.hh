// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Mathematics/Vector2.hh>
#include <Cell/Mathematics/Vector3.hh>
#include <Cell/Mathematics/Vector4.hh>

namespace Cell::Vulkan {

// Represents a single vertex.
struct Vertex {
    // X right, Y down, Z forward
    Mathematics::Vector3 position;

    // RGBA
    Mathematics::Vector4 color;

    // U right, V down
    Mathematics::Vector2 textureCoordinates;
};

}
