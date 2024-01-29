// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Collection/List.hh>
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
    void* parameters;
};

// Manages a command pool and its buffers.
class CommandBufferManager : public Object {
friend Device;

public:
    // Destructs the command pool and all its buffers.
    CELL_FUNCTION ~CommandBufferManager();

    // Creates a series of new buffers.
    CELL_FUNCTION Result CreateBuffers(const uint32_t count);

    // Resets the pool and its buffers.
    CELL_FUNCTION Result Reset(const bool release_resources = true);

    // Begins writing commands to the desired buffer.
    CELL_FUNCTION Result BeginCommands(const uint32_t index);

    // Writes a series of commands to the desired buffer.
    CELL_FUNCTION Result WriteCommands(const uint32_t index, const Command* commands, const uint32_t count);

    // Ends writing commands to the desired buffer.
    CELL_FUNCTION Result EndCommands(const uint32_t index);

    // Writes a series of commands for the desired buffer in one shot.
    // The set must be complete; it'll be wrapped in calls to vkBeginCommandBuffer and vkEndCommandBuffer.
    CELL_FUNCTION Result WriteCommandsSingle(const uint32_t index, const Command* commands, const uint32_t count);

    // Synchronously queues the buffer for execution.
    CELL_FUNCTION Result Submit(const uint32_t index);

    // Returns the owning device of this manager.
    CELL_INLINE Device* GetDevice() { return this->device; }

    // Returns the buffer for the given index.
    CELL_INLINE VkCommandBuffer GetCommandBufferHandle(const uint32_t index) {
        CELL_ASSERT(this->buffers.GetCount() > index);

        return this->buffers[index];
    }

private:
    CommandBufferManager(Device* dev, VkCommandPool pool, VkQueue queue) : device(dev), pool(pool), queue(queue) { }

    Device* device;
    VkCommandPool pool;
    VkQueue queue;

    Collection::List<VkCommandBuffer> buffers;
};

}
