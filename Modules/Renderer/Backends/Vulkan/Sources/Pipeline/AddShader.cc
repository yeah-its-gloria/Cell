// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Renderer/Vulkan/Pipeline.hh>

namespace Cell::Renderer::Vulkan {

Result Pipeline::AddShader(const Memory::IBlock& block, const Stage stage) {
    const VkShaderModuleCreateInfo moduleInfo = {
        .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext    = nullptr,
        .flags    = 0,

        .codeSize = block.GetSize(),
        .pCode    = (const uint32_t*)block.AsPointer()
    };

    VkShaderModule module = nullptr;
    const VkResult result = vkCreateShaderModule(this->device->device, &moduleInfo, nullptr, &module);
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkCreateShaderModule failed");
    }
    }

    const VkShaderStageFlagBits bits = [&]() {
        switch (stage) {
        case Stage::Vertex: {
            return VK_SHADER_STAGE_VERTEX_BIT;
        }

        case Stage::Fragment: {
            return VK_SHADER_STAGE_FRAGMENT_BIT;
        }

        default: {
            CELL_UNIMPLEMENTED
        }
        }
    }();

    const VkPipelineShaderStageCreateInfo stageInfo = {
        .sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext               = nullptr,
        .flags               = 0,

        .stage               = bits,
        .module              = module,
        .pName               = "main",

        .pSpecializationInfo = nullptr
    };

    this->shaders.Append(module);
    this->stages.Append(stageInfo);

    return Result::Success;
}

}
