// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Tools.hh"

using namespace Cell;
using namespace Cell::Vulkan;

void VulkanToolsSetUpResources(Pipeline* pipeline, Buffer** uniforms, Image* texture, IRenderTarget* target) {
    ResourceBinding bindings[2] = {
        { .type = ResourceType::Buffer, .stage = Stage::Vertex },
        { .type = ResourceType::Image, .stage = Stage::Fragment }
    };

    Collection::List<ResourceDescriptor> descriptors(2 * target->GetImageCount()); // 2 resources per image

    for (size_t i = 0; i < descriptors.GetCount(); i += 2) {
        descriptors[i].buffer = uniforms[i / 2];
        descriptors[i].bufferOffset = 0;
        descriptors[i].bufferRange = sizeof(ExampleUBO);

        descriptors[i + 1].image = texture;
        descriptors[i + 1].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }

    Collection::Array bindingsArray(bindings, 2);
    const Result result = pipeline->AddResources(bindingsArray, descriptors);
    CELL_ASSERT(result == Result::Success);
}
