// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Pipeline.hh>
#include <Cell/Vulkan/Vertex.hh>

namespace Cell::Vulkan {

Result Pipeline::Finalize() {
    if (this->stages.GetCount() == 0) {
        return Result::InvalidState;
    }

    // pipeline layout

    List<VkDescriptorSetLayout> setLayouts;
    if (this->resources.GetCount() > 0) {
        for (pipelineResource resource : this->resources) {
            setLayouts.Append(resource.layout);
        }
    }

    const VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext                  = nullptr,
        .flags                  = 0,

        .setLayoutCount         = (uint32_t)setLayouts.GetCount(),
        .pSetLayouts            = &setLayouts,

        .pushConstantRangeCount = 0,
        .pPushConstantRanges    = nullptr
    };

    VkResult result = vkCreatePipelineLayout(this->instance->device, &pipelineLayoutInfo, nullptr, &this->layout);
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
        System::Panic("vkCreatePipelineLayout failed");
    }
    }

    // pipeline data

    const VkVertexInputBindingDescription vertexInputBindingDescription = {
        .binding   = 0,
        .stride    = sizeof(Vertex),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    };

    const VkVertexInputAttributeDescription vertexInputAttributeDescriptions[3] = {
        // position
        {
            .location = 0,
            .binding  = 0,
            .format   = VK_FORMAT_R32G32B32_SFLOAT,
            .offset   = offsetof(Vertex, position)
        },

        // color
        {
            .location = 1,
            .binding  = 0,
            .format   = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset   = offsetof(Vertex, color)
        },

        // texture coordinates
        {
            .location = 2,
            .binding  = 0,
            .format   = VK_FORMAT_R32G32_SFLOAT,
            .offset   = offsetof(Vertex, textureCoordinates)
        }
    };

    const VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
        .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext                           = nullptr,
        .flags                           = 0,

        .vertexBindingDescriptionCount   = 1,
        .pVertexBindingDescriptions      = &vertexInputBindingDescription,

        .vertexAttributeDescriptionCount = 3,
        .pVertexAttributeDescriptions    = vertexInputAttributeDescriptions,
    };

    const VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext                  = nullptr,
        .flags                  = 0,

        .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    };

    const VkViewport viewport = {
        .x        = 0.f,
        .y        = 0.f,
        .width    = 1.f,
        .height   = 1.f,
        .minDepth = 0.f,
        .maxDepth = 1.f
    };

    const VkRect2D scissor = {
        .offset = { 0, 0 },
        .extent = { 1, 1 }
    };

    const VkPipelineViewportStateCreateInfo viewportInfo = {
        .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pNext         = nullptr,
        .flags         = 0,

        .viewportCount = 1,
        .pViewports    = &viewport,
        .scissorCount  = 1,
        .pScissors     = &scissor
    };

    const VkPipelineRasterizationStateCreateInfo rasterizationInfo = {
        .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext                   = nullptr,
        .flags                   = 0,

        .depthClampEnable        = VK_TRUE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode             = VK_POLYGON_MODE_FILL,
        .cullMode                = this->cullMode,
        .frontFace               = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable         = VK_FALSE,
        .depthBiasConstantFactor = 0.f,
        .depthBiasClamp          = 0.f,
        .depthBiasSlopeFactor    = 0.f,
        .lineWidth               = 1.f,
    };

    const VkPipelineMultisampleStateCreateInfo multisamplingInfo = {
        .sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext                 = nullptr,
        .flags                 = 0,

        .rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable   = VK_FALSE,
        .minSampleShading      = 1.0f,
        .pSampleMask           = nullptr,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable      = VK_FALSE
    };

    const VkPipelineDepthStencilStateCreateInfo depthStencilInfo = {
        .sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .pNext                 = nullptr,
        .flags                 = 0,

        .depthTestEnable       = VK_TRUE,
        .depthWriteEnable      = VK_TRUE,

        .depthCompareOp        = VK_COMPARE_OP_LESS,

        .depthBoundsTestEnable = VK_FALSE,

        .stencilTestEnable     = VK_FALSE,
        .front                 = { },
        .back                  = { },

        .minDepthBounds        = 0.0f,
        .maxDepthBounds        = 1.0f
    };

    const VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {
        .blendEnable         = VK_FALSE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
        .colorBlendOp        = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp        = VK_BLEND_OP_ADD,
        .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    };

    const VkPipelineColorBlendStateCreateInfo colorBlendStateInfo = {
        .sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext             = nullptr,
        .flags             = 0,

        .logicOpEnable     = VK_FALSE,
        .logicOp           = VK_LOGIC_OP_COPY,
        .attachmentCount   = 1,
        .pAttachments      = &colorBlendAttachmentState,
        .blendConstants[0] = 0.f,
        .blendConstants[1] = 0.f,
        .blendConstants[2] = 0.f,
        .blendConstants[3] = 0.f,
    };

    const VkDynamicState dynamicStates[2] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    const VkPipelineDynamicStateCreateInfo dynamicStateInfo = {
        .sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .pNext             = nullptr,
        .flags             = 0,

        .dynamicStateCount = 2,
        .pDynamicStates    = dynamicStates
    };

    const VkPipelineRenderingCreateInfoKHR pipelineRenderInfo = {
        .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR,
        .pNext                   = nullptr,

        .viewMask                = 0,
        .colorAttachmentCount    = 1,
        .pColorAttachmentFormats = &this->renderFormat,
        .depthAttachmentFormat   = VK_FORMAT_D32_SFLOAT,
        .stencilAttachmentFormat = VK_FORMAT_UNDEFINED
    };

    // pipeline creation

    const VkGraphicsPipelineCreateInfo pipelineInfo = {
        .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext               = &pipelineRenderInfo,
        .flags               = 0,

        .stageCount          = (uint32_t)this->stages.GetCount(),
        .pStages             = &this->stages,

        .pVertexInputState   = &vertexInputInfo,
        .pInputAssemblyState = &inputAssemblyInfo,
        .pViewportState      = &viewportInfo,
        .pRasterizationState = &rasterizationInfo,
        .pMultisampleState   = &multisamplingInfo,
        .pDepthStencilState  = &depthStencilInfo,
        .pColorBlendState    = &colorBlendStateInfo,
        .pDynamicState       = &dynamicStateInfo,

        .layout              = this->layout,

        .renderPass          = nullptr,
        .subpass             = 0,

        .basePipelineHandle  = nullptr,
        .basePipelineIndex   = -1
    };

    result = vkCreateGraphicsPipelines(this->instance->device, nullptr, 1, &pipelineInfo, nullptr, &this->pipeline);
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
        System::Panic("vkCreateGraphicsPipelines failed");
    }
    }

    return Result::Success;
}

}
