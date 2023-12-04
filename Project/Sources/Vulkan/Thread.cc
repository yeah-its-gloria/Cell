// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Tools.hh"

#include <Cell/Scoped.hh>
#include <Cell/Mathematics/Utilities.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Timer.hh>
#include <Cell/Utilities/MinMaxClamp.hh>
#include <Cell/Vulkan/WSITarget.hh>

using namespace Cell;
using namespace Cell::Mathematics;
using namespace Cell::Vulkan;

void Example::VulkanThread() {
    ScopedObject<Instance> instance = Instance::New().Unwrap();

    Result result = instance->InitializeDevice();
    CELL_ASSERT(result == Result::Success);

    ScopedObject<WSITarget> target = instance->CreateWSITarget(this->shell).Unwrap();

    result = target->SetUpRendering();
    CELL_ASSERT(result == Result::Success);

    VkExtent2D extent = target->GetExtent();

    ScopedObject<Image> lesbianTexture = VulkanToolsLoadTexture(&instance, this->GetContentPath("/Textures/Raw/lesbian.bin"));
    ScopedObject<Image> transTexture = VulkanToolsLoadTexture(&instance, this->GetContentPath("/Textures/Raw/trans.bin"));

    constexpr size_t vertexCount = 16;
    constexpr size_t indexCount = 36;

    ScopedObject<Buffer> buffer = instance->CreateBuffer(sizeof(Vertex) * vertexCount + sizeof(uint16_t) * indexCount,
                                                         VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT).Unwrap();

    Collection::List<Buffer*> uniforms(target->GetImageCount());
    for (uint32_t index = 0; index < uniforms.GetCount(); index++) {
        uniforms[index] = instance->CreateBuffer(sizeof(ExampleUBO), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT).Unwrap();
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

    buffer->Copy(System::UnownedBlock { vertices, vertexCount });
    buffer->Copy(System::UnownedBlock { indices, indexCount }, sizeof(Vertex) * vertexCount);

    ScopedObject<Pipeline> pipeline = instance->CreatePipeline(&target).Unwrap();

    VulkanToolsSetUpResources(&pipeline, uniforms, &lesbianTexture, &transTexture, &target);

    VulkanToolsLoadShader(&pipeline, this->GetContentPath("/Shaders/Default.spv"));

    result = pipeline->Finalize();
    CELL_ASSERT(result == Result::Success);

    ScopedObject<CommandBufferManager> cmdBufferManager = instance->CreateCommandBufferManager().Unwrap();

    result = cmdBufferManager->CreateBuffers(target->GetImageCount());
    CELL_ASSERT(result == Result::Success);

    ExampleUBO ubo;

    ubo.model.SetToIdentity();

    //ubo.view.LookAt(Vector3 { 0.f, 5.f, 5.f }, Mathematics::Utilities::DegreesToRadians(10.f), Mathematics::Utilities::DegreesToRadians(90.f));
    ubo.view.LookAt({ 0.f, 0.1f, -5.f }, { 0.f, 0.f, 0.f }, { 0.f, 0.f, -1.f });

    uint64_t finishedTick = System::GetPreciseTickerValue();
    while (this->shell->IsStillActive()) {
        this->renderDeltaTime = Cell::Utilities::Minimum(( System::GetPreciseTickerValue() - finishedTick) / 1000.f, 0.001f);

        ubo.timeMilliseconds = this->elapsedTime;
        ubo.projection.Perspective(Mathematics::Utilities::DegreesToRadians(45.f), (float)extent.width / (float)extent.height, 0.1, 1000.f);

        this->inputMutex.Lock();

        ubo.projection.Rotate(this->rotationX, { 0.f, 1.f, 0.f });
        ubo.projection.Rotate(this->rotationY, { 1.f, 0.f, 0.f });
        ubo.projection.Translate(this->position);

        this->inputMutex.Unlock();

        uniforms[target->GetFrameCounter()]->Copy(System::UnownedBlock { &ubo });

        VulkanToolsGenerateRenderCommands(vertexCount, indexCount, &cmdBufferManager, &pipeline, &buffer, &target, target->GetFrameCounter());

        result = instance->RenderImage(&target, cmdBufferManager->GetCommandBufferHandle(target->GetFrameCounter()));
        switch (result) {
        case Result::Success: {
            break;
        }

        case Result::Suboptimal: {
            result = target->Recreate();
            CELL_ASSERT(result == Result::Success);

            extent = target->GetExtent();
            break;
        }

        default: {
            System::Panic("Cell::Vulkan::Instance::RenderImage failed");
        }
        }

        finishedTick = System::GetPreciseTickerValue();

        System::Thread::Yield();
    }

    for (Buffer* uniform : uniforms) {
        delete uniform;
    }
}
