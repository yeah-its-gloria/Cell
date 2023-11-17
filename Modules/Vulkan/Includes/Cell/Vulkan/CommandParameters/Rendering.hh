// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <vulkan/vulkan.h>

namespace Cell::Vulkan::CommandParameters {

// Begins rendering.
struct BeginRendering {
    const VkRect2D renderArea;
    const uint32_t layerCount;
    const uint32_t viewMask;
    const uint32_t colorAttachmentCount;
    const VkRenderingAttachmentInfo* colorAttachments;
    const VkRenderingAttachmentInfo* depthAttachments;
    const VkRenderingAttachmentInfo* stencilAttachments;
};

}
