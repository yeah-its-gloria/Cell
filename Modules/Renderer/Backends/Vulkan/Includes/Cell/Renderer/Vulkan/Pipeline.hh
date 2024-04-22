// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Collection/List.hh>
#include <Cell/Mathematics/Vector2.hh>
#include <Cell/Mathematics/Quaternion.hh>
#include <Cell/Renderer/Vulkan/Buffer.hh>
#include <Cell/Renderer/Vulkan/Image.hh>

namespace Cell::Renderer::Vulkan {

enum class ResourceType : uint8_t {
    Buffer,
    Image
};

struct ResourceBinding {
    ResourceType type;
    Stage stage;
};

struct ResourceDescriptor {
    size_t bindingIndex;

    Buffer* buffer;
    VkDeviceSize bufferRange;
    VkDeviceSize bufferOffset;

    Image* image;
    VkImageLayout imageLayout;
};

// Represents a single vertex.
struct Vertex {
    // X right, Y down, Z forward
    Mathematics::Vector3 position;

    // RGBA
    Mathematics::Quaternion color;

    // U right, V down
    Mathematics::Vector2 textureCoordinates;

    uint32_t textureIndex;
};

class Pipeline : public NoCopyObject {
friend Device;

public:
    // Destructor for pipelines.
    CELL_FUNCTION ~Pipeline();

    // Adds a shader stage to the pipeline. The module is created with the passed SPIR-V bytecode and its lifetime is managed by this pipeline manager.
    // By default, it's a vertex shader.
    CELL_FUNCTION Result AddShader(const Memory::IBlock& data, const Stage stage = Stage::Vertex);

    // Adds a shader with vertex and fragment stages to the pipeline. The module is owned by the pipeline manager.
    CELL_FUNCTION Result AddMultiShader(const Memory::IBlock& data);

    // Adds resources for shaders to this pipeline.
    CELL_FUNCTION Result AddResources(const Collection::IEnumerable<ResourceBinding>& resBindings, const Collection::IEnumerable<ResourceDescriptor>& resDescriptors);

    // Finalizes the pipeline data into a proper pipeline.
    CELL_FUNCTION Result Finalize();

    // Returns the owning device.
    CELL_FUNCTION_TEMPLATE Device* GetDevice() { return this->device; }

    // Returns a handle to the pipeline object.
    CELL_FUNCTION_TEMPLATE VkPipeline GetPipelineHandle() { return this->pipeline; }

    // Returns a handle to the pipeline object.
    CELL_FUNCTION_TEMPLATE VkPipelineLayout GetPipelineLayoutHandle() { return this->layout; }

    // Returns the descriptor sets for the given resource index.
    CELL_FUNCTION_TEMPLATE VkDescriptorSet* GetDescriptorSets(const uint32_t index) { return this->resources[index].sets; }

private:
    CELL_FUNCTION_INTERNAL Pipeline(Device* dev, VkFormat fmt) : device(dev), renderFormat(fmt) { }

    struct PipelineResource {
        VkDescriptorSetLayout layout;
        VkDescriptorPool pool;
        VkDescriptorSet* sets;
    };

    Device* device;
    VkFormat renderFormat;

    VkPipelineLayout layout = nullptr;
    VkPipeline pipeline = nullptr;

    Collection::List<PipelineResource> resources;
    Collection::List<VkPipelineShaderStageCreateInfo> stages;
    Collection::List<VkShaderModule> shaders;
};

}
