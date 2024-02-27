// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/CommandBuffer.hh>

namespace Cell::Vulkan {

using namespace CommandParameters;

Result CommandBuffer::Write(const Collection::IEnumerable<const Command>& commands) {
    if (this->recordState != RecordState::Recording) {
        return Result::InvalidState;
    }

    for (const Command& command : commands) {
        switch (command.type) {
        case CommandType::BindPipeline: {
            BindPipeline* parameters = (BindPipeline*)command.parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdBindPipeline(this->buffer, parameters->point, parameters->pipeline);
            break;
        }

        case CommandType::BindVertexBuffers: {
            BindVertexBuffers* parameters = (BindVertexBuffers*)command.parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdBindVertexBuffers(this->buffer, parameters->bindingFirst, parameters->bindingCount, parameters->buffers, parameters->offsets);
            break;
        }

        case CommandType::BindIndexBuffer: {
            BindIndexBuffer* parameters = (BindIndexBuffer*)command.parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdBindIndexBuffer(this->buffer, parameters->buffer, parameters->offset, parameters->type);
            break;
        }

        case CommandType::BindDescriptorSets: {
            BindDescriptorSets* parameters = (BindDescriptorSets*)command.parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdBindDescriptorSets(this->buffer, parameters->point, parameters->layout, parameters->setFirstIndex, parameters->setCount, parameters->sets, parameters->dynamicOffsetCount,
                                    parameters->dynamicOffsets);
            break;
        }

        case CommandType::SetViewport: {
            VkViewport* parameters = (VkViewport*)command.parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdSetViewport(this->buffer, 0, 1, parameters);
            break;
        }

        case CommandType::SetScissor: {
            VkRect2D* parameters = (VkRect2D*)command.parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdSetScissor(this->buffer, 0, 1, parameters);
            break;
        }

        case CommandType::SetCullMode: {
            VkCullModeFlags cullMode = *(VkCullModeFlags*)command.parameters;
            this->device->instance->setCullMode(this->buffer, cullMode);
            break;
        }

        case CommandType::Draw: {
            Draw* parameters = (Draw*)command.parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdDraw(this->buffer, parameters->vertexCount, parameters->instanceCount, parameters->vertexFirstIndex, parameters->instanceFirstIndex);
            break;
        }

        case CommandType::DrawIndexed: {
            DrawIndexed* parameters = (DrawIndexed*)command.parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdDrawIndexed(this->buffer, parameters->indexCount, parameters->instanceCount, parameters->indexFirstIndex, parameters->vertexOffset, parameters->instanceFirstIndex);
            break;
        }

        case CommandType::InsertBarrier: {
            InsertBarrier* parameters = (InsertBarrier*)command.parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdPipelineBarrier(this->buffer, parameters->stageMaskSource, parameters->stageMaskDestination, parameters->dependencyFlags, parameters->barrierCount, parameters->barriers,
                                 parameters->bufferCount, parameters->buffers, parameters->imageCount, parameters->images);
            break;
        }

        case CommandType::CopyBuffer: {
            CopyBuffer* parameters = (CopyBuffer*)command.parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdCopyBuffer(this->buffer, parameters->source, parameters->destination, parameters->regionCount, parameters->regions);
            break;
        }

        case CommandType::CopyBufferToImage: {
            CopyBufferToImage* parameters = (CopyBufferToImage*)command.parameters;
            if (parameters == nullptr) {
                return Result::InvalidCommandGiven;
            }

            vkCmdCopyBufferToImage(this->buffer, parameters->source, parameters->destination, parameters->layout, parameters->regionCount, parameters->regions);
            break;
        }

        case CommandType::BeginRendering: {
            BeginRendering* parameters = (BeginRendering*)command.parameters;
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

            this->device->instance->beginRendering(this->buffer, &renderingInfo);
            break;
        }

        case CommandType::EndRendering: {
            if (command.parameters != nullptr) {
                return Result::InvalidCommandGiven;
            }

            this->device->instance->endRendering(this->buffer);
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
