// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Collection/Enumerable.hh>
#include <Cell/Vulkan/Device.hh>

#include <Cell/Vulkan/CommandParameters/Binding.hh>
#include <Cell/Vulkan/CommandParameters/Copy.hh>
#include <Cell/Vulkan/CommandParameters/Draw.hh>
#include <Cell/Vulkan/CommandParameters/Rendering.hh>

namespace Cell::Vulkan {

// Represents different available commands and their parameter blocks.
enum class CommandType : uint8_t {
    BindPipeline,       // BindPipeline
    BindVertexBuffers,  // BindVertexBuffers
    BindIndexBuffer,    // BindIndexBuffer
    BindDescriptorSets, // BindDescriptorSets
    SetViewport,        // VkViewport
    SetScissor,         // VkRect2D
    SetCullMode,        // VkCullModeFlags
    Draw,               // Draw
    DrawIndexed,        // DrawIndexed
    InsertBarrier,      // InsertBarrier
    CopyBuffer,         // CopyBuffer
    CopyBufferToImage,  // CopyBufferToImage
    BeginRendering,     // BeginRendering
    EndRendering        // nullptr
};

// Represents a single command.
struct Command {
    // The command itself to run.
    CommandType type;

    // The parameters for the command.
    const void* parameters;
};

class CommandBuffer : public NoCopyObject {
friend Device;

public:
    // Begins recording commands.
    CELL_FUNCTION Result Begin();

    // Finishes recording commands.
    CELL_FUNCTION Result End();

    // Resets the contents of this buffer.
    CELL_FUNCTION Result Reset();

    // Writes a series of commands to the buffer.
    CELL_FUNCTION Result Write(const Collection::IEnumerable<const Command>& commands);

    // Begins recording, writes a series of commands, and ends recording this buffer, in one pass.
    CELL_FUNCTION Result WriteSinglePass(const Collection::IEnumerable<const Command>& commands);

    // Submits this buffer for synchronous execution.
    CELL_FUNCTION Result Submit();

    // Submits this buffer for drawing to the given target, using the target's synchronization capabilities, if available.
    CELL_FUNCTION Result Submit(IRenderTarget* CELL_NONNULL target);

private:
    enum class RecordState : uint8_t {
        Initialized,
        Recording,
        Recorded
    };

    CELL_FUNCTION_INTERNAL CommandBuffer(VkCommandBuffer b, VkQueue& q, Device* d) : buffer(b), queueRef(q), device(d)  { }

    VkCommandBuffer buffer;
    VkQueue& queueRef;

    RecordState recordState = RecordState::Recorded;

    Device* device;
};

}
