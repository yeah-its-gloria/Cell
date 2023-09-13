// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Example.hh"
#include "Tools.hh"

#include <Cell/Scoped.hh>
#include <Cell/Mathematics/Utilities.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Ticker.hh>
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

    ScopedObject<Image> texture = VulkanToolsLoadTexture(&instance, this->GetContentPath("/Textures/Raw/trans.bin"));

    constexpr size_t vertexCount = 8;
    constexpr size_t indexCount = 36;

    ScopedObject<Buffer> buffer = instance->CreateBuffer(sizeof(Vertex) * vertexCount + sizeof(uint16_t) * indexCount,
                                                         VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT).Unwrap();

    List<Buffer*> uniforms(target->GetImageCount());
    for (uint32_t index = 0; index < uniforms.GetCount(); index++) {
        uniforms[index] = instance->CreateBuffer(sizeof(ExampleUBO), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT).Unwrap();
    }

    const Vertex vertices[vertexCount] = {
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
        4, 5, 7
    };

    buffer->Copy((void*)vertices, sizeof(Vertex) * 8);
    buffer->Copy((void*)indices, sizeof(uint16_t) * indexCount, sizeof(Vertex) * 8);

    ScopedObject<Pipeline> pipeline = instance->CreatePipeline(&target).Unwrap();
    pipeline->SetCullingMode(CullMode::Back);

    VulkanToolsSetUpResources(&pipeline, &uniforms, &texture, &target);

    VulkanToolsLoadShader(&pipeline, this->GetContentPath("/Shaders/DefaultVertex.spv"), Stage::Vertex);
    VulkanToolsLoadShader(&pipeline, this->GetContentPath("/Shaders/DefaultFragment.spv"), Stage::Fragment);

    result = pipeline->Finalize();
    CELL_ASSERT(result == Result::Success);

    ScopedObject<CommandBufferManager> cmdBufferManager = instance->CreateCommandBufferManager().Unwrap();

    result = cmdBufferManager->CreateBuffers(target->GetImageCount());
    CELL_ASSERT(result == Result::Success);

    Vector3 position = {0.f, 0.f, 0.f};

    ExampleUBO ubo;
    ubo.projection.Perspective(Mathematics::Utilities::DegreesToRadians(45.f), (float)extent.width / (float)extent.height, 0.1, 1000.f);

    InputData inputData = {&position, this};
    VulkanToolsInputSetUp(this->input, &inputData);

    uint64_t finishedTick = System::GetPreciseTickerValue();
    while (this->shell->IsStillActive()) {
        this->renderDeltaTime = Cell::Utilities::Minimum((System::GetPreciseTickerValue() - finishedTick) / 1000.f, 0.005f);

        ubo.timeMilliseconds = this->elapsedTime;

        ubo.model.SetToIdentity();

        ubo.view.LookAt(Vector3 {0.f, 5.f, 5.f}, Vector3 {0.f, 0.f, 0.f}, Vector3 {0.f, 0.f, -1.f});
        //ubo.view.LookAt(Vector3 { 0.f, 5.f, 5.f }, Mathematics::Utilities::DegreesToRadians(10.f), Mathematics::Utilities::DegreesToRadians(90.f));
        ubo.view.Translate(position);

        uniforms[target->GetFrameCount()]->Copy(&ubo, sizeof(ExampleUBO));

        VulkanToolsGenerateRenderCommands(vertexCount, indexCount, &cmdBufferManager, &pipeline, &buffer, &target, target->GetFrameCount());
        result = instance->RenderImage(&target, cmdBufferManager->GetCommandBufferHandle(target->GetFrameCount()));
        switch (result) {
        case Result::Success: {
            break;
        }

        case Result::Suboptimal: {
            result = target->Recreate();
            CELL_ASSERT(result == Result::Success);

            extent = target->GetExtent();
            ubo.projection.Perspective(Mathematics::Utilities::DegreesToRadians(45.f), (float)extent.width / (float)extent.height, 0.1, 10.f);
            break;
        }

        default: {
            System::Panic("Cell::Vulkan::Instance::RenderImage failed");
        }
        }

        Shell::Result shellResult = input->Poll();
        CELL_ASSERT(shellResult == Shell::Result::Success);

        finishedTick = System::GetPreciseTickerValue();
    }

    for (Buffer* uniform : uniforms) {
        delete uniform;
    }
}
