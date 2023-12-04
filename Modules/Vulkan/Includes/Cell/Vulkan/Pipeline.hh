// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Collection/List.hh>
#include <Cell/Mathematics/Vector2.hh>
#include <Cell/Mathematics/Vector3.hh>
#include <Cell/Mathematics/Vector4.hh>
#include <Cell/System/Block.hh>
#include <Cell/Vulkan/Buffer.hh>
#include <Cell/Vulkan/Image.hh>

namespace Cell::Vulkan {

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
    Mathematics::Vector4 color;

    // U right, V down
    Mathematics::Vector2 textureCoordinates;

    uint32_t textureIndex;
};

class Pipeline : public Object {
friend Instance;

public:
    // Destructor for pipelines.
    CELL_FUNCTION ~Pipeline();

    // Adds a shader stage to the pipeline. The module is created with the passed SPIR-V bytecode and its lifetime is managed by this pipeline manager.
    // By default, it's a vertex shader.
    CELL_FUNCTION Result AddShader(const IBlock& data, const Stage stage = Stage::Vertex);

    // Adds a shader with vertex and fragment stages to the pipeline. The module is owned by the pipeline manager.
    CELL_FUNCTION Result AddMultiShader(const IBlock& data);

    // Adds resources for shaders to this pipeline.
    CELL_FUNCTION Result AddResources(Collection::IEnumerable<ResourceBinding>& resBindings, Collection::IEnumerable<ResourceDescriptor>& resDescriptors);

    // Finalizes the pipeline data into a proper pipeline.
    CELL_FUNCTION Result Finalize();

    // Returns the owning instance.
    CELL_INLINE Instance* GetOwningInstance() { return this->instance; }

    // Returns a handle to the pipeline object.
    CELL_INLINE VkPipeline GetPipelineHandle() { return this->pipeline; }

    // Returns a handle to the pipeline object.
    CELL_INLINE VkPipelineLayout GetPipelineLayoutHandle() { return this->layout; }

    // Returns the descriptor sets for the given resource index.
    CELL_INLINE VkDescriptorSet* GetDescriptorSets(const uint32_t index) { return this->resources[index].sets; }

private:
    CELL_FUNCTION_INTERNAL Pipeline(Instance* instance, VkFormat renderFormat) : instance(instance), layout(nullptr), pipeline(nullptr), renderFormat(renderFormat) { }

    struct PipelineResource {
        VkDescriptorSetLayout layout;
        VkDescriptorPool pool;
        VkDescriptorSet* sets;
    };

    Instance* instance;
    VkPipelineLayout layout;
    VkPipeline pipeline;

    VkFormat renderFormat;

    Collection::List<PipelineResource> resources;
    Collection::List<VkPipelineShaderStageCreateInfo> stages;
    Collection::List<VkShaderModule> shaders;
};

}
