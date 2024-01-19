// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Vulkan/Tools.hh"
#include "Tools.hh"

#include <Cell/Mathematics/Utilities.hh>
#include <Cell/OpenXR/VulkanTarget.hh>
#include <Cell/System/BlockImpl.hh>

using namespace Cell;

void XRToolsPrepare(Example* example,
                    OpenXR::Instance* instance,
                    ExampleUBO& ubo,
                    Vulkan::Image* texture1,
                    Vulkan::Image* texture2,
                    Vulkan::Buffer* buffer,
                    Collection::List<Vulkan::Buffer*>& uniforms,
                    Vulkan::Pipeline* pipeline,
                    Vulkan::CommandBufferManager* commandBuffer,
                    OpenXR::VulkanTarget* target) {
    Vulkan::Instance* vkInstance = instance->GetVulkan();

    texture1 = VulkanToolsLoadTexture(vkInstance, example->GetContentPath("/Textures/Raw/lesbian.bin"));
    texture2 = VulkanToolsLoadTexture(vkInstance, example->GetContentPath("/Textures/Raw/lesbian.bin"));

    constexpr size_t indexCount = 36;

    buffer = vkInstance->CreateBuffer(sizeof(Vulkan::Vertex) * 8 + sizeof(uint16_t) * indexCount,
                                      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT).Unwrap();

    uniforms.SetCount(target->GetImageCount());
    for (uint32_t index = 0; index < uniforms.GetCount(); index++) {
        uniforms[index] = vkInstance->CreateBuffer(sizeof(ExampleUBO), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT).Unwrap();
    }

    const Vulkan::Vertex vertices[8] = {
        { { -1.0f, -1.0f, -1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 0.0f, 1.0f }, 0 },
        { {  1.0f, -1.0f, -1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 1.0f, 1.0f }, 0 },
        { { -1.0f,  1.0f, -1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 0.0f, 0.0f }, 0 },
        { {  1.0f,  1.0f, -1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 1.0f, 0.0f }, 0 },

        { { -1.0f, -1.0f,  1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 0.0f, 1.0f }, 0 },
        { {  1.0f, -1.0f,  1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 1.0f, 1.0f }, 0 },
        { { -1.0f,  1.0f,  1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 0.0f, 0.0f }, 0 },
        { {  1.0f,  1.0f,  1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 1.0f, 0.0f }, 0 },
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

    Vulkan::Result result = buffer->Copy(System::UnownedBlock { vertices, 8 });
    CELL_ASSERT(result == Vulkan::Result::Success);

    result = buffer->Copy(System::UnownedBlock { indices, indexCount }, sizeof(Vulkan::Vertex) * 8);
    CELL_ASSERT(result == Vulkan::Result::Success);

    pipeline = vkInstance->CreatePipeline(target).Unwrap();

    VulkanToolsSetUpResources(pipeline, uniforms, texture1, texture2, target);

    VulkanToolsLoadShader(pipeline, example->GetContentPath("/Shaders/Default.spv"));

    result = pipeline->Finalize();
    CELL_ASSERT(result == Vulkan::Result::Success);

    commandBuffer = vkInstance->CreateCommandBufferManager().Unwrap();

    result = commandBuffer->CreateBuffers(target->GetImageCount());
    CELL_ASSERT(result == Vulkan::Result::Success);

    VkExtent2D extent = target->GetExtent();
    ubo.projection.Perspective(Mathematics::Utilities::DegreesToRadians(45.f), (float)extent.width / (float)extent.height, 0.1, 1000.f);
}
