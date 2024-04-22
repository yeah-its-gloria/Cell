// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Tools.hh"

#include <Cell/Collection/Array.hh>
#include <Cell/System/Log.hh>

using namespace Cell;
using namespace Cell::Renderer;
using namespace Cell::Renderer::Vulkan;

void VulkanToolsSetUpResources(Pipeline* pipeline, Collection::List<Buffer*>& uniforms, Image* texture1, Image* texture2, IRenderTarget* target) {
    const Collection::List<ResourceBinding> bindings = {
        { .type = ResourceType::Buffer, .stage = Stage::Vertex   },
        { .type = ResourceType::Image,  .stage = Stage::Fragment },
        { .type = ResourceType::Image,  .stage = Stage::Fragment }
    };

    Collection::List<ResourceDescriptor> descriptors(3 * target->GetImageCount()); // 3 resources per image

    for (size_t i = 0; i < descriptors.GetCount(); i += 3) {
        descriptors[i].buffer = uniforms[i / 3];
        descriptors[i].bufferOffset = 0;
        descriptors[i].bufferRange = sizeof(ExampleUBO);

        descriptors[i + 1].image = texture1;
        descriptors[i + 1].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        descriptors[i + 2].image = texture2;
        descriptors[i + 2].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }

    const Result result = pipeline->AddResources(bindings, descriptors);
    CELL_ASSERT(result == Result::Success);
}
