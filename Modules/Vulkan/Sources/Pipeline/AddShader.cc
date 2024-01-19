// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Pipeline.hh>

namespace Cell::Vulkan {

Result Pipeline::AddShader(const IBlock& block, const Stage stage) {
    const VkShaderModuleCreateInfo moduleInfo =
    {
        .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext    = nullptr,
        .flags    = 0,

        .codeSize = block.ByteSize(),
        .pCode    = (const uint32_t*)block.Pointer()
    };

    VkShaderModule module = nullptr;
    const VkResult result = vkCreateShaderModule(this->instance->device, &moduleInfo, nullptr, &module);
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

    VkShaderStageFlagBits bits;
    switch (stage) {
    case Stage::Vertex: {
        bits = VK_SHADER_STAGE_VERTEX_BIT;
        break;
    }

    case Stage::Fragment: {
        bits = VK_SHADER_STAGE_FRAGMENT_BIT;
        break;
    }

    default: {
        CELL_UNIMPLEMENTED
    }
    }

    const VkPipelineShaderStageCreateInfo stageInfo =
    {
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
