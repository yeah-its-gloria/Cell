// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <vulkan/vulkan.h>

namespace Cell::Vulkan::CommandParameters {

// Binds a pipeline to the command buffer.
struct BindPipeline {
    VkPipeline pipeline;
    const VkPipelineBindPoint point;
};

// Binds vertex buffers to the command buffer.
struct BindVertexBuffers {
    const uint32_t bindingFirst;
    const uint32_t bindingCount;
    const VkBuffer* buffers;
    const VkDeviceSize* offsets;
};

// Binds an index buffers to the command buffer.
struct BindIndexBuffer {
    VkBuffer buffer;
    const VkDeviceSize offset;
    const VkIndexType type;
};

// Binds descriptor sets to the command buffer.
struct BindDescriptorSets {
    const VkPipelineBindPoint point;
    VkPipelineLayout layout;
    const VkDescriptorSet* sets;
    const uint32_t setCount;
    const uint32_t setFirstIndex;
    const uint32_t dynamicOffsetCount;
    const uint32_t* dynamicOffsets;
};

}
