// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/CommandBufferManager.hh>

namespace Cell::Vulkan {

using namespace CommandParameters;

Result CommandBufferManager::WriteCommands(const uint32_t index, const Command* commands, const uint32_t count) {
    if (this->buffers.GetCount() < index) {
        return Result::InvalidParameters;
    }

    for (uint32_t i = 0; i < count; i++) {
        switch (commands[i].type) {
        case CommandType::BindPipeline: {
            BindPipeline* parameters = (BindPipeline*)commands[i].parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdBindPipeline(this->buffers[index], parameters->point, parameters->pipeline);
            break;
        }

        case CommandType::BindVertexBuffers: {
            BindVertexBuffers* parameters = (BindVertexBuffers*)commands[i].parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdBindVertexBuffers(this->buffers[index], parameters->bindingFirst, parameters->bindingCount, parameters->buffers, parameters->offsets);
            break;
        }

        case CommandType::BindIndexBuffer: {
            BindIndexBuffer* parameters = (BindIndexBuffer*)commands[i].parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdBindIndexBuffer(this->buffers[index], parameters->buffer, parameters->offset, parameters->type);
            break;
        }

        case CommandType::BindDescriptorSets: {
            BindDescriptorSets* parameters = (BindDescriptorSets*)commands[i].parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdBindDescriptorSets(this->buffers[index], parameters->point, parameters->layout, parameters->setFirstIndex, parameters->setCount, parameters->sets, parameters->dynamicOffsetCount,
                                    parameters->dynamicOffsets);
            break;
        }

        case CommandType::SetViewport: {
            VkViewport* parameters = (VkViewport*)commands[i].parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdSetViewport(this->buffers[index], 0, 1, parameters);
            break;
        }

        case CommandType::SetScissor: {
            VkRect2D* parameters = (VkRect2D*)commands[i].parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdSetScissor(this->buffers[index], 0, 1, parameters);
            break;
        }

        case CommandType::Draw: {
            Draw* parameters = (Draw*)commands[i].parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdDraw(this->buffers[index], parameters->vertexCount, parameters->instanceCount, parameters->vertexFirstIndex, parameters->instanceFirstIndex);
            break;
        }

        case CommandType::DrawIndexed: {
            DrawIndexed* parameters = (DrawIndexed*)commands[i].parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdDrawIndexed(this->buffers[index], parameters->indexCount, parameters->instanceCount, parameters->indexFirstIndex, parameters->vertexOffset, parameters->instanceFirstIndex);
            break;
        }

        case CommandType::InsertBarrier: {
            InsertBarrier* parameters = (InsertBarrier*)commands[i].parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdPipelineBarrier(this->buffers[index], parameters->stageMaskSource, parameters->stageMaskDestination, parameters->dependencyFlags, parameters->barrierCount, parameters->barriers,
                                 parameters->bufferCount, parameters->buffers, parameters->imageCount, parameters->images);
            break;
        }

        case CommandType::CopyBuffer: {
            CopyBuffer* parameters = (CopyBuffer*)commands[i].parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdCopyBuffer(this->buffers[index], parameters->source, parameters->destination, parameters->regionCount, parameters->regions);
            break;
        }

        case CommandType::CopyBufferToImage: {
            CopyBufferToImage* parameters = (CopyBufferToImage*)commands[i].parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdCopyBufferToImage(this->buffers[index], parameters->source, parameters->destination, parameters->layout, parameters->regionCount, parameters->regions);
            break;
        }

        case CommandType::BeginRendering: {
            BeginRendering* parameters = (BeginRendering*)commands[i].parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            const VkRenderingInfo renderingInfo = {
                .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
                .pNext = nullptr,
                .flags = 0,

                .renderArea           = parameters->renderArea,
                .layerCount           = parameters->layerCount,
                .viewMask             = parameters->viewMask,
                .colorAttachmentCount = parameters->colorAttachmentCount,
                .pColorAttachments    = parameters->colorAttachments,
                .pDepthAttachment     = parameters->depthAttachments,
                .pStencilAttachment   = parameters->stencilAttachments
            };

            this->instance->beginRendering(this->buffers[index], &renderingInfo);
            break;
        }

        case CommandType::EndRendering: {
            if (commands[i].parameters != nullptr) {
                return Result::InvalidCommandGiven;
            }

            this->instance->endRendering(this->buffers[index]);
            break;
        }

        default: {
            return Result::InvalidCommandGiven;
        }
        }
    }

    return Result::Success;
}

}
