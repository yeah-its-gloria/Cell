// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Tools.hh"

using namespace Cell;
using namespace Cell::Vulkan;

void VulkanToolsSetUpResources(Pipeline* pipeline, Buffer** uniforms, Image* texture, IRenderTarget* target) {
    const PipelineResourceData uniformData = {
        .type = PipelineResourceType::Buffer,
        .stage = Stage::Vertex,

        .buffer = nullptr,
        .bufferRange = sizeof(ExampleUBO),
        .bufferOffset = 0,

        .image = nullptr,
        .imageLayout = VK_IMAGE_LAYOUT_UNDEFINED
    };

    const PipelineResourceData textureData = {
        .type = PipelineResourceType::Image,
        .stage = Stage::Fragment,

        .buffer = nullptr,
        .bufferRange = 0,
        .bufferOffset = 0,

        .image = texture,
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };

    // awful hacks

    List<PipelineResourceData*> resourcesList(target->GetImageCount());

    uint32_t uniformCounter = 0;
    for (PipelineResourceData*& resources : resourcesList) {
        resources = System::AllocateMemory<PipelineResourceData>(2);

        System::CopyMemory<PipelineResourceData>(&resources[0], &uniformData);
        System::CopyMemory<PipelineResourceData>(&resources[1], &textureData);

        resources[0].buffer = uniforms[uniformCounter++];
    }

    const Result result = pipeline->AddResources(&resourcesList, 2, resourcesList.GetCount());
    CELL_ASSERT(result == Result::Success);

    for (PipelineResourceData*& resources : resourcesList) {
        System::FreeMemory(resources);
    }
}
