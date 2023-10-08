// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Dictionary.hh>
#include <Cell/Vulkan/Pipeline.hh>

namespace Cell::Vulkan {

Result Pipeline::AddResources(PipelineResourceData** resources, const uint32_t resourceCount, const uint32_t setCount) {
    if (resourceCount == 0 || setCount == 0) {
        return Result::InvalidParameters;
    }

    // binding generation

    List<VkDescriptorSetLayoutBinding> bindings;
    for (uint32_t i = 0; i < resourceCount; i++) {
        VkDescriptorType type;
        switch (resources[0][i].type) {
        case PipelineResourceType::Buffer: {
            type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            break;
        }

        case PipelineResourceType::Image: {
            type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            break;
        }

        default: {
            CELL_UNIMPLEMENTED
        }
        }

        VkShaderStageFlags stageFlags;
        switch (resources[0][i].stage) {
        case Stage::Vertex: {
            stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            break;
        }

        case Stage::Fragment: {
            stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            break;
        }

        default: {
            CELL_UNIMPLEMENTED
        }
        }

        const VkDescriptorSetLayoutBinding binding = {
            .binding         = i,
            .descriptorType  = type,
            .descriptorCount = 1,
            .stageFlags      = stageFlags
        };

        bindings.Append(binding);
    }

    // layout creation

    const VkDescriptorSetLayoutCreateInfo layoutInfo = {
        .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext        = nullptr,
        .flags        = 0,

        .bindingCount = (uint32_t)bindings.GetCount(),
        .pBindings    = &bindings
    };

    VkDescriptorSetLayout layout = nullptr;
    VkResult result = vkCreateDescriptorSetLayout(this->instance->device, &layoutInfo, nullptr, &layout);
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
        System::Panic("vkCreateDescriptorSetLayout failed");
    }
    }

    // pool allocation

    Dictionary<VkDescriptorType, uint32_t> countPerType;
    for (VkDescriptorSetLayoutBinding binding : bindings) {
        Optional<size_t> position = countPerType.Has(binding.descriptorType);
        if (position) {
            countPerType.Set(position, (countPerType.GetValue(position) + 1) * setCount);
            continue;
        }

        countPerType.Append(binding.descriptorType, setCount);
    }

    List<VkDescriptorPoolSize> poolSizes(countPerType.GetCount());
    for (size_t i = 0; i < countPerType.GetCount(); i++) {
        poolSizes[i].descriptorCount = countPerType.GetValue(i);
        poolSizes[i].type = countPerType.GetKey(i);
    }

    const VkDescriptorPoolCreateInfo poolInfo = {
        .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext         = nullptr,
        .flags         = 0,

        .maxSets       = setCount,
        .poolSizeCount = (uint32_t)poolSizes.GetCount(),
        .pPoolSizes    = &poolSizes
    };

    VkDescriptorPool pool = nullptr;
    result = vkCreateDescriptorPool(this->instance->device, &poolInfo, nullptr, &pool);
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroyDescriptorSetLayout(this->instance->device, layout, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroyDescriptorSetLayout(this->instance->device, layout, nullptr);
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkCreateDescriptorPool failed");
    }
    }

    // set allocation

    List<VkDescriptorSetLayout> layouts(layout, setCount);

    const VkDescriptorSetAllocateInfo setAllocateInfo = {
        .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext              = nullptr,

        .descriptorPool     = pool,
        .descriptorSetCount = setCount,
        .pSetLayouts        = &layouts
    };

    VkDescriptorSet* sets = System::AllocateMemory<VkDescriptorSet>(setCount);
    result = vkAllocateDescriptorSets(this->instance->device, &setAllocateInfo, sets);
    switch (result) {
    case VK_SUCCESS: {
        break;
    }

    case VK_ERROR_OUT_OF_HOST_MEMORY: {
        vkDestroyDescriptorPool(this->instance->device, pool, nullptr);
        vkDestroyDescriptorSetLayout(this->instance->device, layout, nullptr);
        return Result::OutOfHostMemory;
    }

    case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
        vkDestroyDescriptorPool(this->instance->device, pool, nullptr);
        vkDestroyDescriptorSetLayout(this->instance->device, layout, nullptr);
        return Result::OutOfDeviceMemory;
    }

    default: {
        System::Panic("vkAllocateDescriptorSets failed");
    }
    }

    // set resource writing

    List<VkWriteDescriptorSet> writeSets;
    List<VkDescriptorBufferInfo> bufferInfos;
    List<VkDescriptorImageInfo> imageInfos;

    for (uint32_t setIndex = 0; setIndex < setCount; setIndex++) {
        for (uint32_t resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++) {
            VkWriteDescriptorSet set = {
                .sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext            = nullptr,
                .dstSet           = sets[setIndex],
                .dstBinding       = resourceIndex,
                .dstArrayElement  = 0,
                .descriptorCount  = 1,
                .descriptorType   = bindings[resourceIndex].descriptorType,
                .pImageInfo       = nullptr,
                .pBufferInfo      = nullptr,
                .pTexelBufferView = nullptr
            };

            switch (bindings[resourceIndex].descriptorType) {
            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER: {
                const VkDescriptorBufferInfo bufferInfo = {
                    .buffer = resources[setIndex][resourceIndex].buffer->buffer,
                    .offset = resources[setIndex][resourceIndex].bufferOffset,
                    .range  = resources[setIndex][resourceIndex].bufferRange
                };

                const size_t bufferInfoPos = bufferInfos.Append(bufferInfo);
                set.pBufferInfo = bufferInfos.GetPointer(bufferInfoPos);
                break;
            }

            case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: {
                const VkDescriptorImageInfo imageInfo = {
                    .sampler     = resources[setIndex][resourceIndex].image->sampler,
                    .imageView   = resources[setIndex][resourceIndex].image->view,
                    .imageLayout = resources[setIndex][resourceIndex].imageLayout
                };

                const size_t imageInfoPos = imageInfos.Append(imageInfo);
                set.pImageInfo = imageInfos.GetPointer(imageInfoPos);
                break;
            }

            default: {
                CELL_UNIMPLEMENTED
            }
            }

            writeSets.Append(set);
        }
    }

    // BUG: (Linux) this crashes with a page fault, quit using a double list
    vkUpdateDescriptorSets(this->instance->device, writeSets.GetCount(), &writeSets, 0, nullptr);

    // collecting data

    this->resources.Append({ .layout = layout, .pool = pool, .sets = sets });
    return Result::Success;
}

}
