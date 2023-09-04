// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Pipeline.hh>

namespace Cell::Vulkan {

void Pipeline::SetCullingMode(const CullMode mode) {
    switch (mode) {
    case CullMode::Front: {
        this->cullMode = VK_CULL_MODE_FRONT_BIT;
        break;
    }

    case CullMode::Back: {
        this->cullMode = VK_CULL_MODE_BACK_BIT;
        break;
    }

    case CullMode::None: {
        this->cullMode = VK_CULL_MODE_NONE;
        break;
    }

    default: {
        CELL_UNREACHABLE;
    }
    }
}

}
