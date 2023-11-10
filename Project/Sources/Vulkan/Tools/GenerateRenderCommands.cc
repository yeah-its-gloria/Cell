// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Tools.hh"

using namespace Cell::Vulkan;
using namespace Cell::Vulkan::CommandParameters;

void VulkanToolsGenerateRenderCommands(const uint32_t vertexCount,
                                       const uint32_t drawCount,
                                             CommandBufferManager* commandBuffer,
                                             Pipeline* pipeline,
                                             Buffer* buffer,
                                             IRenderTarget* target,
                                       const uint32_t frameId) {
    Result result = commandBuffer->Reset();
    CELL_ASSERT(result == Result::Success);

    const VkExtent2D extent = target->GetExtent();

    const VkImageMemoryBarrier drawingBarrierImage = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = nullptr,
        .srcAccessMask = VK_ACCESS_NONE,
        .dstAccessMask = VK_ACCESS_NONE,
        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .srcQueueFamilyIndex = 0,
        .dstQueueFamilyIndex = 0,
        .image = target->GetColorImage(frameId),
        .subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .subresourceRange.baseMipLevel = 0,
        .subresourceRange.levelCount = 1,
        .subresourceRange.baseArrayLayer = 0,
        .subresourceRange.layerCount = 1
    };

    CommandParameters::InsertBarrier drawingBarrier = {
        .stageMaskSource = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .stageMaskDestination = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        .dependencyFlags = 0,
        .barrierCount = 0,
        .barriers = nullptr,
        .bufferCount = 0,
        .buffers = nullptr,
        .imageCount = 1,
        .images = &drawingBarrierImage
    };

    const VkClearValue colorValue = { .color = { { 0.0f, 0.0f, 0.0f, 0.0f } } };
    const VkClearValue depthValue = { .depthStencil = { 1.0f, 0 } };

    const VkRenderingAttachmentInfo colorAttachment = {
        .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .pNext = nullptr,

        .imageView = target->GetColorImageView(frameId),
        .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,

        .resolveMode = VK_RESOLVE_MODE_NONE,
        .resolveImageView = nullptr,
        .resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,

        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,

        .clearValue = colorValue
    };

    const VkRenderingAttachmentInfo depthAttachment = {
        .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .pNext = nullptr,

        .imageView = target->GetDepthImageView(frameId),
        .imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,

        .resolveMode = VK_RESOLVE_MODE_NONE,
        .resolveImageView = nullptr,
        .resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,

        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,

        .clearValue = depthValue
    };

    CommandParameters::BeginRendering renderingParameters = {
        .renderArea.extent = extent,
        .renderArea.offset = { 0, 0 },

        .layerCount = 1,
        .viewMask = 0,

        .colorAttachmentCount = 1,
        .colorAttachments = &colorAttachment,
        .depthAttachments = &depthAttachment,
        .stencilAttachments = nullptr
    };

    CommandParameters::BindPipeline pipelineParameters = {
        .pipeline = pipeline->GetPipelineHandle(),
        .point = VK_PIPELINE_BIND_POINT_GRAPHICS
    };

    VkBuffer bufferHandle = buffer->GetBufferHandle();
    VkDeviceSize offset = 0;

    CommandParameters::BindVertexBuffers vertexParameters = {
        .bindingFirst = 0,
        .bindingCount = 1,
        .buffers = &bufferHandle,
        .offsets = &offset
    };

    CommandParameters::BindIndexBuffer indexParameters = {
        .buffer = bufferHandle,
        .offset = sizeof(Vertex) * vertexCount,
        .type = VK_INDEX_TYPE_UINT16
    };

    VkDescriptorSet set = pipeline->GetDescriptorSets(0)[frameId];
    CommandParameters::BindDescriptorSets setsParameters = {
        .point = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .layout = pipeline->GetPipelineLayoutHandle(),
        .sets = &set,
        .setCount = 1,
        .setFirstIndex = 0,
        .dynamicOffsetCount = 0,
        .dynamicOffsets = nullptr
    };

    VkViewport viewport = {
        .x = 0.f,
        .y = 0.f,
        .width = (float)extent.width,
        .height = (float)extent.height,
        .minDepth = 0.f,
        .maxDepth = 1.f
    };

    VkRect2D scissor = {
        .offset = { 0, 0 },
        .extent = extent
    };

    VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT;

    CommandParameters::DrawIndexed drawParameters = {
        .indexCount = drawCount,
        .instanceCount = 1,
        .indexFirstIndex = 0,
        .vertexOffset = 0,
        .instanceFirstIndex = 0
    };

    const VkImageMemoryBarrier presentationBarrierImage = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = nullptr,
        .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        .dstAccessMask = VK_ACCESS_NONE,
        .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        .srcQueueFamilyIndex = 0,
        .dstQueueFamilyIndex = 0,
        .image = target->GetColorImage(frameId),
        .subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .subresourceRange.baseMipLevel = 0,
        .subresourceRange.levelCount = 1,
        .subresourceRange.baseArrayLayer = 0,
        .subresourceRange.layerCount = 1
    };

    CommandParameters::InsertBarrier presentationBarrier = {
        .stageMaskSource = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .stageMaskDestination = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
        .dependencyFlags = 0,
        .barrierCount = 0,
        .barriers = nullptr,
        .bufferCount = 0,
        .buffers = nullptr,
        .imageCount = 1,
        .images = &presentationBarrierImage
    };

    Command commands[12] = {
        { CommandType::InsertBarrier,      &drawingBarrier },
        { CommandType::BeginRendering,     &renderingParameters },
        { CommandType::BindPipeline,       &pipelineParameters },
        { CommandType::BindVertexBuffers,  &vertexParameters },
        { CommandType::BindIndexBuffer,    &indexParameters },
        { CommandType::BindDescriptorSets, &setsParameters },
        { CommandType::SetViewport,        &viewport },
        { CommandType::SetScissor,         &scissor },
        { CommandType::SetCullMode,        &cullMode },
        { CommandType::DrawIndexed,        &drawParameters },
        { CommandType::EndRendering,       nullptr },
        { CommandType::InsertBarrier,      &presentationBarrier }
    };

    result = commandBuffer->WriteCommandsSingle(frameId, commands, 12);
    CELL_ASSERT(result == Result::Success);
}
