// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Pipeline.hh>

namespace Cell::Vulkan {

Wrapped<Pipeline*, Result> Device::CreatePipeline(IRenderTarget* target) {
    return new Pipeline(this, target->GetColorFormat());
}

Pipeline::~Pipeline() {
    if (this->pipeline != nullptr) {
        vkDestroyPipeline(this->device->device, this->pipeline, nullptr);
    }

    if (this->layout != nullptr) {
        vkDestroyPipelineLayout(this->device->device, this->layout, nullptr);
    }

    if (this->resources.GetCount() > 0) {
        for (PipelineResource resource : this->resources) {
            System::FreeMemory(resource.sets);

            vkDestroyDescriptorPool(this->device->device, resource.pool, nullptr);
            vkDestroyDescriptorSetLayout(this->device->device, resource.layout, nullptr);
        }
    }

    if (this->shaders.GetCount() > 0) {
        for (VkShaderModule shader : this->shaders) {
            vkDestroyShaderModule(this->device->device, shader, nullptr);
        }
    }
}

}
