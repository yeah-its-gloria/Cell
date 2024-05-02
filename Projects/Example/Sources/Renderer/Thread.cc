// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Tools.hh"
#include "../InputController.hh"

#include <Cell/Scoped.hh>
#include <Cell/Mathematics/Utilities.hh>
#include <Cell/Memory/OwnedBlock.hh>
#include <Cell/Memory/UnownedBlock.hh>
#include <Cell/System/Timer.hh>
#include <Cell/Utilities/MinMaxClamp.hh>
#include <Cell/Renderer/Vulkan/WSITarget.hh>

using namespace Cell;
using namespace Cell::Mathematics;
using namespace Cell::Mathematics::Utilities;
using namespace Cell::Renderer;
using namespace Cell::Renderer::Vulkan;

void Example::RendererThread() {
    Shell::Result shellResult = this->shell->IndicateStatus(Shell::ShellStatus::Working);
    CELL_ASSERT(shellResult == Shell::Result::Success);

    ScopedObject instance = Instance::New().Unwrap();
    ScopedObject device = instance->CreateDevice().Unwrap();
    ScopedObject target = device->CreateWSITarget(this->shell).Unwrap();

    VkExtent2D extent = target->GetExtent();

    ScopedObject lesbianTexture = VulkanToolsLoadTexture(&device, this->GetContentPath("/Textures/LesbianTex.bin"));
    ScopedObject transTexture = VulkanToolsLoadTexture(&device, this->GetContentPath("/Textures/TransTex.bin"));

    constexpr size_t vertexCount = 16;
    constexpr size_t indexCount = 36;

    ScopedObject buffer = device->CreateBuffer(sizeof(Vertex) * vertexCount + sizeof(uint16_t) * indexCount,
                                                         VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT).Unwrap();

    Collection::List<Buffer*> uniforms(target->GetImageCount());
    for (uint32_t index = 0; index < uniforms.GetCount(); index++) {
        uniforms[index] = device->CreateBuffer(sizeof(ExampleUBO), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT).Unwrap();
    }

    const Vertex vertices[vertexCount] = {
        { { -1.0f, -1.0f, -1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 0.0f, 1.0f }, 0 },
        { {  1.0f, -1.0f, -1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 1.0f, 1.0f }, 0 },
        { { -1.0f,  1.0f, -1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 0.0f, 0.0f }, 0 },
        { {  1.0f,  1.0f, -1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 1.0f, 0.0f }, 0 },

        { { -1.0f, -1.0f,  1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 0.0f, 1.0f }, 0 },
        { {  1.0f, -1.0f,  1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 1.0f, 1.0f }, 0 },
        { { -1.0f,  1.0f,  1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 0.0f, 0.0f }, 0 },
        { {  1.0f,  1.0f,  1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 1.0f, 0.0f }, 0 },

        { { -1.0f, -1.0f, -1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 0.0f, 0.0f }, 1 },
        { {  1.0f, -1.0f, -1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 1.0f, 0.0f }, 1 },
        { { -1.0f, -1.0f,  1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 0.0f, 1.0f }, 1 },
        { {  1.0f, -1.0f,  1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 1.0f, 1.0f }, 1 },

        { { -1.0f,  1.0f, -1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 0.0f, 0.0f }, 1 },
        { {  1.0f,  1.0f, -1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 1.0f, 0.0f }, 1 },
        { { -1.0f,  1.0f,  1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 0.0f, 1.0f }, 1 },
        { {  1.0f,  1.0f,  1.0f }, { 1.f, 1.f, 1.f, 1.f }, { 1.0f, 1.0f }, 1 },
    };

    const uint16_t indices[indexCount] = {
        // front
        0, 2, 1,
        1, 2, 3,

        // bottom
        8, 9, 10,
        10, 9, 11,

        // left
        0, 4, 6,
        6, 2, 0,

        // top
        14, 15, 12,
        12, 15, 13,

        // right
        3, 7, 5,
        5, 1, 3,

        // top
        4, 7, 6,
        4, 5, 7
    };

    buffer->Copy(Memory::UnownedBlock { vertices, vertexCount });
    buffer->Copy(Memory::UnownedBlock { indices, indexCount }, sizeof(Vertex) * vertexCount);

    ScopedObject pipeline = device->CreatePipeline(&target).Unwrap();

    VulkanToolsSetUpResources(&pipeline, uniforms, &lesbianTexture, &transTexture, &target);

    //VulkanToolsLoadShader(&pipeline, this->GetContentPath("/Shaders/Default.spv"));

    VulkanToolsLoadShader(&pipeline, this->GetContentPath("/Shaders/DefaultVertex.spv"), Stage::Vertex);
    VulkanToolsLoadShader(&pipeline, this->GetContentPath("/Shaders/DefaultFragment.spv"), Stage::Fragment);

    Result result = pipeline->Finalize();
    CELL_ASSERT(result == Result::Success);

    Collection::List<CommandBuffer*> commandBuffers(target->GetImageCount());
    for (uint32_t index = 0; index < uniforms.GetCount(); index++) {
        commandBuffers[index] = device->CreateCommandBuffer().Unwrap();
    }

    ExampleUBO ubo = {
        .model = Matrix4x4::FromIdentity().Translate({ 0, 0, -5 }),
        .view = Matrix4x4(),
        .projection = Matrix4x4::AsPerspective(DegreesToRadians(70.f), (float)extent.width / (float)extent.height, 0.1f, 1000.f),
        .delta = 0.f
    };

    shellResult = this->shell->IndicateStatus(Shell::ShellStatus::Default);
    CELL_ASSERT(shellResult == Shell::Result::Success);

    shellResult = this->shell->CaptureState(true);
    CELL_ASSERT(shellResult == Shell::Result::Success);

    uint64_t finishedTick = System::GetPreciseTickerValue();
    while (this->shell->IsStillActive()) {
        this->renderDeltaTime = Cell::Utilities::Minimum((System::GetPreciseTickerValue() - finishedTick) / 1000.f, 0.001f);

        if (!this->shell->IsInForeground()) {
            System::SleepPrecise(400);
            continue;
        }

        ubo.delta = this->renderDeltaTime;
        ubo.view = this->controller->GetCamera();

        const uint32_t frameIndex = target->GetFrameCounter();

        uniforms[frameIndex]->Copy(Memory::UnownedBlock { &ubo });
        VulkanToolsGenerateRenderCommands(vertexCount, indexCount, commandBuffers[frameIndex], &pipeline, &buffer, &target, frameIndex);

        result = commandBuffers[frameIndex]->Submit(&target);
        switch (result) {
        case Result::Success: {
            break;
        }

        case Result::Suboptimal: {
            result = target->Recreate();
            CELL_ASSERT(result == Result::Success);

            extent = target->GetExtent();
            ubo.projection = Matrix4x4::AsPerspective(DegreesToRadians(70.f), (float)extent.width / (float)extent.height, 0.1, 1000.f);
            break;
        }

        default: {
            System::Panic("Cell::Renderer::Vulkan::Instance::RenderImage failed");
        }
        }

        finishedTick = System::GetPreciseTickerValue();
        System::SleepPrecise(400);
    }
}
