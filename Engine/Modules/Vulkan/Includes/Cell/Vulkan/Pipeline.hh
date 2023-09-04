// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "Cell/System/Block.hh"
#include <Cell/List.hh>
#include <Cell/Vulkan/Buffer.hh>
#include <Cell/Vulkan/Image.hh>

namespace Cell::Vulkan {

enum class PipelineResourceType : uint8_t {
    Buffer,
    Image
};

struct PipelineResourceData {
    PipelineResourceType type;
    Stage stage;

    Buffer* buffer;
    VkDeviceSize bufferRange;
    VkDeviceSize bufferOffset;

    Image* image;
    VkImageLayout imageLayout;
};

enum class CullMode : uint8_t {
    Front,
    Back,
    None
};

class Pipeline : public Object {
    friend Instance;

public:
    // Destructor for pipelines.
    CELL_FUNCTION ~Pipeline();

    // Adds a shader stage to the pipeline. The module is created with the passed SPIR-V bytecode and its lifetime is managed by this pipeline manager.
    // By default, it's a vertex shader.
    CELL_FUNCTION Result AddShader(const IBlock& data, const Stage stage = Stage::Vertex);

    // Adds the given resources from the list.
    // The resources need to be a two-dimensional list, resources per set ([setCount][resourceCount]).
    CELL_FUNCTION Result AddResources(PipelineResourceData** resources, const uint32_t resourceCount, const uint32_t setCount = 1);

    // Sets a new culling mode for this pipeline. Default is culling the back of faces.
    CELL_FUNCTION void SetCullingMode(const CullMode mode);

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
    CELL_FUNCTION_INTERNAL Pipeline(Instance* instance, VkFormat renderFormat) : instance(instance), layout(nullptr), pipeline(nullptr), cullMode(VK_CULL_MODE_NONE), renderFormat(renderFormat) { }

    struct pipelineResource {
        VkDescriptorSetLayout layout;
        VkDescriptorPool pool;
        VkDescriptorSet* sets;
    };

    Instance* instance;
    VkPipelineLayout layout;
    VkPipeline pipeline;

    VkCullModeFlags cullMode;
    VkFormat renderFormat;

    List<pipelineResource> resources;
    List<VkPipelineShaderStageCreateInfo> stages;
    List<VkShaderModule> shaders;
};

}
