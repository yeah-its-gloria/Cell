// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/BlockImpl.hh>
#include <Cell/Vulkan/Instance.hh>

namespace Cell::Vulkan {

Wrapped<Instance::QueryPhysicalDeviceResult, Result> Instance::QueryPhysicalDevice(VkPhysicalDevice CELL_NULLABLE device) {
    VkPhysicalDevice physicalDevice = nullptr;

    if (device == nullptr) {
        uint32_t deviceCount = 0;
        VkResult vk_result = vkEnumeratePhysicalDevices(this->instance, &deviceCount, nullptr);
        switch (vk_result) {
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
            System::Panic("vkEnumeratePhysicalDevices failed");
        }
        }

        System::OwnedBlock<VkPhysicalDevice> devices(deviceCount);
        vk_result = vkEnumeratePhysicalDevices(this->instance, &deviceCount, devices);
        switch (vk_result) {
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
            System::Panic("vkEnumeratePhysicalDevices failed");
        }
        }

        uint32_t index = (uint32_t)-1;
        uint16_t score = 0;

        for (uint32_t i = 0; i < deviceCount; i++) {
            const uint16_t temp_score = Instance::ScorePhysicalDevice(devices[i]);
            if (temp_score > score) {
                score = temp_score;
                index = i;
            }
        }

        if (index == (uint32_t)-1) {
            return Result::RunningOnIncompatibleHardware;
        }

        physicalDevice = devices[index];
    } else {
        physicalDevice = device;
    }

    const Instance::PhysicalDeviceQueues queues = Instance::QueryPhysicalDeviceQueues(physicalDevice);

    Instance::QueryPhysicalDeviceResult result = {
        .device = physicalDevice,
        .queues = queues
    };

    vkGetPhysicalDeviceMemoryProperties(result.device, &result.properties);
    return result;
}

}
