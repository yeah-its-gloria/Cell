// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Tools.hh"

#include <Cell/Scoped.hh>
#include <Cell/OpenXR/VulkanTarget.hh>
#include <Cell/System/Log.hh>

using namespace Cell;
using namespace Cell::Mathematics;

void Example::XRThread() {
    Wrapped<OpenXR::Instance*, OpenXR::Result> instanceResult = OpenXR::Instance::New();
    if (!instanceResult.IsValid()) {
        switch (instanceResult.Result()) {
        case OpenXR::Result::Success: {
            break;
        }

        case OpenXR::Result::RuntimeUnavailable: {
            System::Log("Failed to initialize OpenXR; runtime isn't available.");
            return;
        }

        case OpenXR::Result::RuntimeFailure: {
            System::Log("Failed to initialize OpenXR; runtime isn't functioning.");
            return;
        }

        default: {
            System::Panic("Cell::OpenXR::Instance::New failed");
        }
        }
    }

    ScopedObject<OpenXR::Instance> instance = instanceResult.Unwrap();

    OpenXR::Result result = instance->InitializeSystem();
    switch (result) {
    case OpenXR::Result::Success: {
        break;
    }

    case OpenXR::Result::Unavailable: {
        System::Log("Failed to initialize OpenXR system; no headset connected.");
        return;
    }

    default: {
        System::Panic("Cell::OpenXR::Instance::InitializeSystem failed");
    }
    }

    result = instance->FetchViewConfiguration();
    CELL_ASSERT(result == OpenXR::Result::Success);

    result = instance->InitializeSessionVulkan();
    CELL_ASSERT(result == OpenXR::Result::Success);

    result = instance->CreateSpace();
    CELL_ASSERT(result == OpenXR::Result::Success);

    result = instance->CreateRenderTargetsVulkan();
    CELL_ASSERT(result == OpenXR::Result::Success);

    const VkExtent2D resolution = instance->GetVulkanTarget(0)->GetExtent();
    const Shell::Result shellResult = this->shell->SetDrawableExtent({ resolution.width / 2, resolution.height / 2 });
    CELL_ASSERT(shellResult == Shell::Result::Success);

    ExampleUBO ubo;

    ubo.model.SetToIdentity();
    ubo.projection.Perspective(instance->GetFoVForEye(0), (float)resolution.width / (float)resolution.height, 0.1, 1000.f);

    ScopedObject<Vulkan::Image> texture = nullptr;
    ScopedObject<Vulkan::Buffer> buffer = nullptr;
    ScopedObject<Vulkan::Pipeline> pipeline = nullptr;

    ScopedObject<Vulkan::CommandBufferManager> commandBuffer[2] = {nullptr, nullptr};
    Collection::List<Vulkan::Buffer*> uniforms[2];

    for (uint8_t i = 0; i < OpenXR::ViewCount; i++) {
        XRToolsPrepare(this, &instance, ubo, &texture, &buffer, uniforms[i], &pipeline, &commandBuffer[i], instance->GetVulkanTarget(i));
    }

    bool active = false;
    while (this->shell->IsStillActive()) {
        result = instance->UpdateState();
        CELL_ASSERT(result == OpenXR::Result::Success || result == OpenXR::Result::Unavailable);

        switch (instance->GetSessionState()) {
        case OpenXR::SessionState::Idle: {
            break;
        }

        case OpenXR::SessionState::Ready: {
            if (!active) {
                result = instance->BeginSession();
                CELL_ASSERT(result == OpenXR::Result::Success);

                active = true;
            }

            break;
        }

        case OpenXR::SessionState::Stopping: {
            if (active) {
                result = instance->EndSession();
                CELL_ASSERT(result == OpenXR::Result::Success);

                active = false;
            }
        }

        case OpenXR::SessionState::Synchronized:
        case OpenXR::SessionState::Focussed:
        case OpenXR::SessionState::Visible:
        case OpenXR::SessionState::Ending:
        case OpenXR::SessionState::Exiting: {
            break;
        }

        default: {
            CELL_UNREACHABLE;
        }
        }

        if (active) {
            result = instance->BeginFrame();
            if (result == OpenXR::Result::ViewOrPoseMalformed) {
                System::Thread::Yield();
                continue;
            }

            CELL_ASSERT(result == OpenXR::Result::Success);

            for (uint8_t i = 0; i < OpenXR::ViewCount; i++) {
                ubo.view = (Matrix4x4(instance->GetViewOrientation(i)) * Matrix4x4(instance->GetViewPosition(i))).Invert();

                const uint32_t index = instance->GetVulkanTarget(i)->GetCurrentImageIndex();
                uniforms[i][index]->Copy(&ubo, sizeof(ExampleUBO));

                VulkanToolsGenerateRenderCommands(8, 36, &commandBuffer[i], &pipeline, &buffer, instance->GetVulkanTarget(i), index);

                Vulkan::Result vkResult = instance->GetVulkan()->RenderImage(instance->GetVulkanTarget(i),
                                                                             commandBuffer[i]->GetCommandBufferHandle(instance->GetVulkanTarget(i)->GetCurrentImageIndex()));
                switch (vkResult) {
                case Vulkan::Result::Success: {
                    break;
                }

                default: {
                    System::Panic("Cell::Vulkan::Instance::RenderImage failed");
                }
                }
            }

            result = instance->EndFrame();
            CELL_ASSERT(result == OpenXR::Result::Success);
        }

        System::Thread::Yield();
    }

    result = instance->RequestExit();
    CELL_ASSERT(result == OpenXR::Result::Success);

    for (Collection::List<Vulkan::Buffer*>& uniform : uniforms) {
        for (Vulkan::Buffer* buffer : uniform) {
            delete buffer;
        }
    }
}
