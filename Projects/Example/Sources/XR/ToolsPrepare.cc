// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Vulkan/Tools.hh"
#include "Tools.hh"
#include <Cell/Mathematics/Utilities.hh>
#include <Cell/OpenXR/VulkanTarget.hh>

using namespace Cell;

void XRToolsPrepare(OpenXR::Instance* instance,
                    ExampleUBO& ubo,
                    Vulkan::Image* texture,
                    Vulkan::Buffer* buffer,
                    List<Vulkan::Buffer*>& uniforms,
                    Vulkan::Pipeline* pipeline,
                    Vulkan::CommandBufferManager* commandBuffer,
                    OpenXR::VulkanTarget* target) {
    Vulkan::Instance* vkInstance = instance->GetVulkan();

    texture = VulkanToolsLoadTexture(vkInstance, "./Projects/Example/Content/Textures/Raw/lesbian.bin");

    constexpr size_t indexCount = 36;

    buffer = vkInstance->CreateBuffer(sizeof(Vulkan::Vertex) * 8 + sizeof(uint16_t) * indexCount,
                                      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
                 .Unwrap();

    uniforms.SetCount(target->GetImageCount());
    for (uint32_t index = 0; index < uniforms.GetCount(); index++) {
        uniforms[index] = vkInstance->CreateBuffer(sizeof(ExampleUBO), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT).Unwrap();
    }

    const Vulkan::Vertex vertices[8] = {
        {{-1.0f, -1.0f, -1.0f}, {1.f, 1.f, 1.f, 1.f}, {0.0f, 1.0f}},
        {{1.0f, -1.0f, -1.0f}, {1.f, 1.f, 1.f, 1.f}, {1.0f, 1.0f}},
        {{-1.0f, 1.0f, -1.0f}, {1.f, 1.f, 1.f, 1.f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, -1.0f}, {1.f, 1.f, 1.f, 1.f}, {1.0f, 0.0f}},

        {{-1.0f, -1.0f, 1.0f}, {1.f, 1.f, 1.f, 1.f}, {0.0f, 1.0f}},
        {{1.0f, -1.0f, 1.0f}, {1.f, 1.f, 1.f, 1.f}, {1.0f, 1.0f}},
        {{-1.0f, 1.0f, 1.0f}, {1.f, 1.f, 1.f, 1.f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 1.0f}, {1.f, 1.f, 1.f, 1.f}, {1.0f, 0.0f}},
    };

    const uint16_t indices[indexCount] = {
        // bottom
        0, 2, 1,
        1, 2, 3,

        // back
        0, 1, 4,
        4, 1, 5,

        // left
        0, 4, 6,
        6, 2, 0,

        // front
        6, 7, 2,
        2, 7, 3,

        // right
        3, 7, 5,
        5, 1, 3,

        // top
        4, 7, 6,
        4, 5, 7};

    Vulkan::Result result = buffer->Copy((void*)vertices, sizeof(Vulkan::Vertex) * 8);
    CELL_ASSERT(result == Vulkan::Result::Success);

    result = buffer->Copy((void*)indices, sizeof(uint16_t) * indexCount, sizeof(Vulkan::Vertex) * 8);
    CELL_ASSERT(result == Vulkan::Result::Success);

    pipeline = vkInstance->CreatePipeline(target).Unwrap();
    //pipeline->SetCullingMode(Vulkan::CullMode::Back);

    VulkanToolsSetUpResources(pipeline, &uniforms, texture, target);

    VulkanToolsLoadShader(pipeline, "./Projects/Example/Content/Shaders/DefaultVertex.spv", Vulkan::Stage::Vertex);
    VulkanToolsLoadShader(pipeline, "./Projects/Example/Content/Shaders/DefaultFragment.spv", Vulkan::Stage::Fragment);

    result = pipeline->Finalize();
    CELL_ASSERT(result == Vulkan::Result::Success);

    commandBuffer = vkInstance->CreateCommandBufferManager().Unwrap();

    result = commandBuffer->CreateBuffers(target->GetImageCount());
    CELL_ASSERT(result == Vulkan::Result::Success);

    VkExtent2D extent = target->GetExtent();
    ubo.projection.Perspective(Mathematics::Utilities::DegreesToRadians(45.f), (float)extent.width / (float)extent.height, 0.1, 1000.f);
}
