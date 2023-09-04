// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <vulkan/vulkan.h>

namespace Cell::Vulkan::CommandParameters {

// Draws the given number of vertices.
struct Draw {
    const uint32_t vertexCount;
    const uint32_t instanceCount;
    const uint32_t vertexFirstIndex;
    const uint32_t instanceFirstIndex;
};

// Draws vertices from a list of indices.
struct DrawIndexed {
    const uint32_t indexCount;
    const uint32_t instanceCount;
    const uint32_t indexFirstIndex;
    const uint32_t vertexOffset;
    const uint32_t instanceFirstIndex;
};

}
