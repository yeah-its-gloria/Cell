// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Pipeline.hh>

namespace Cell::Vulkan {

Pipeline::~Pipeline() {
    if (this->pipeline != nullptr) {
        vkDestroyPipeline(this->instance->device, this->pipeline, nullptr);
    }

    if (this->layout != nullptr) {
        vkDestroyPipelineLayout(this->instance->device, this->layout, nullptr);
    }

    if (this->resources.GetCount() > 0) {
        for (PipelineResource resource : this->resources) {
            System::FreeMemory(resource.sets);

            vkDestroyDescriptorPool(this->instance->device, resource.pool, nullptr);
            vkDestroyDescriptorSetLayout(this->instance->device, resource.layout, nullptr);
        }
    }

    if (this->shaders.GetCount() > 0) {
        for (VkShaderModule shader : this->shaders) {
            vkDestroyShaderModule(this->instance->device, shader, nullptr);
        }
    }
}

}
