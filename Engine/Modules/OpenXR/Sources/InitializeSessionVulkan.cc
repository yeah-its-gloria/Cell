// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/OpenXR/Instance.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Panic.hh>

namespace Cell::OpenXR {

Result Instance::InitializeSessionVulkan() {
    PFN_xrGetVulkanGraphicsRequirementsKHR getVkReqs = nullptr;
    Result result = this->GetFunctionPointer("xrGetVulkanGraphicsRequirementsKHR", (PFN_xrVoidFunction*)&getVkReqs);
    if (result != Result::Success) {
        return result;
    }

    XrGraphicsRequirementsVulkanKHR requirements = {
        .type                   = XR_TYPE_GRAPHICS_REQUIREMENTS_VULKAN_KHR,
        .next                   = nullptr,
        .minApiVersionSupported = 0,
        .maxApiVersionSupported = 0
    };

    XrResult xrResult = getVkReqs(this->instance, this->systemId, &requirements);
    switch (xrResult) {
    case XR_SUCCESS: {
        break;
    }

    case XR_ERROR_RUNTIME_FAILURE: {
        return Result::RuntimeFailure;
    }

    case XR_ERROR_INSTANCE_LOST: {
        return Result::RuntimeLost;
    }

    default: {
        System::Panic("xrGetVulkanGraphicsRequirementsKHR failed");
    }
    }

    PFN_xrGetVulkanInstanceExtensionsKHR getVkInstExt = nullptr;
    result = this->GetFunctionPointer("xrGetVulkanInstanceExtensionsKHR", (PFN_xrVoidFunction*)&getVkInstExt);
    if (result != Result::Success) {
        return result;
    }

    uint32_t instanceExtensionSize = 0;
    xrResult = getVkInstExt(this->instance, this->systemId, 0, &instanceExtensionSize, nullptr);
    switch (xrResult) {
    case XR_SUCCESS: {
        break;
    }

    case XR_ERROR_RUNTIME_FAILURE: {
        return Result::RuntimeFailure;
    }

    case XR_ERROR_INSTANCE_LOST: {
        return Result::RuntimeLost;
    }

    case XR_ERROR_OUT_OF_MEMORY: {
        return Result::OutOfMemory;
    }

    default: {
        System::Panic("xrGetVulkanInstanceExtensionsKHR failed");
    }
    }

    System::OwnedBlock<char> instanceExtensions(instanceExtensionSize);
    xrResult = getVkInstExt(this->instance, this->systemId, instanceExtensionSize, &instanceExtensionSize, instanceExtensions);
    switch (xrResult) {
    case XR_SUCCESS: {
        break;
    }

    case XR_ERROR_RUNTIME_FAILURE: {
        return Result::RuntimeFailure;
    }

    case XR_ERROR_INSTANCE_LOST: {
        return Result::RuntimeLost;
    }

    case XR_ERROR_OUT_OF_MEMORY: {
        return Result::OutOfMemory;
    }

    default: {
        System::Panic("xrGetVulkanInstanceExtensionsKHR failed");
    }
    }

    // TODO: create list dynamically

    const char* instanceExtensionNames[4] = {
        "VK_KHR_external_memory_capabilities",
        "VK_KHR_get_physical_device_properties2",
        "VK_KHR_surface",
        "VK_KHR_win32_surface"
    };

    this->vulkan = Vulkan::Instance::New(instanceExtensionNames, 4).Unwrap();

    PFN_xrGetVulkanGraphicsDeviceKHR getVkPhysDev = nullptr;
    result = this->GetFunctionPointer("xrGetVulkanGraphicsDeviceKHR", (PFN_xrVoidFunction*)&getVkPhysDev);
    if (result != Result::Success) {
        return result;
    }

    xrResult = getVkPhysDev(this->instance, this->systemId, this->vulkan->instance, &this->vulkan->physicalDevice);
    switch (xrResult) {
    case XR_SUCCESS: {
        break;
    }

    case XR_ERROR_RUNTIME_FAILURE: {
        return Result::RuntimeFailure;
    }

    case XR_ERROR_INSTANCE_LOST: {
        return Result::RuntimeLost;
    }

    default: {
        System::Panic("xrGetVulkanGraphicsDeviceKHR failed");
    }
    }

    const uint64_t queues = Vulkan::Instance::QueryPhysicalDeviceQueues(this->vulkan->physicalDevice);
    this->vulkan->physicalDeviceQueueGraphics = queues >> 32;
    this->vulkan->physicalDeviceQueueTransfer = queues & 0xffffffff;

    vkGetPhysicalDeviceMemoryProperties(this->vulkan->physicalDevice, &this->vulkan->physicalDeviceProperties);

    PFN_xrGetVulkanDeviceExtensionsKHR getVkDevExt = nullptr;
    result = this->GetFunctionPointer("xrGetVulkanDeviceExtensionsKHR", (PFN_xrVoidFunction*)&getVkDevExt);
    if (result != Result::Success) {
        return result;
    }

    uint32_t deviceExtensionSize = 0;
    xrResult = getVkDevExt(this->instance, this->systemId, 0, &deviceExtensionSize, nullptr);
    switch (xrResult) {
    case XR_SUCCESS: {
        break;
    }

    case XR_ERROR_RUNTIME_FAILURE: {
        return Result::RuntimeFailure;
    }

    case XR_ERROR_INSTANCE_LOST: {
        return Result::RuntimeLost;
    }

    case XR_ERROR_OUT_OF_MEMORY: {
        return Result::OutOfMemory;
    }

    default: {
        System::Panic("xrGetVulkanDeviceExtensionsKHR failed");
    }
    }

    System::OwnedBlock<char> deviceExtensions(deviceExtensionSize);
    xrResult = getVkDevExt(this->instance, this->systemId, deviceExtensionSize, &deviceExtensionSize, deviceExtensions);
    switch (xrResult) {
    case XR_SUCCESS: {
        break;
    }

    case XR_ERROR_RUNTIME_FAILURE: {
        return Result::RuntimeFailure;
    }

    case XR_ERROR_INSTANCE_LOST: {
        return Result::RuntimeLost;
    }

    case XR_ERROR_OUT_OF_MEMORY: {
        return Result::OutOfMemory;
    }

    default: {
        System::Panic("xrGetVulkanDeviceExtensionsKHR failed");
    }
    }

    const char* deviceExtensionNames[7] = {
        "VK_KHR_external_memory",
        "VK_KHR_external_semaphore",
        "VK_KHR_dedicated_allocation",
        "VK_KHR_get_memory_requirements2",
        "VK_KHR_external_memory_win32",
        "VK_KHR_win32_keyed_mutex",
        VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME
    };

    Vulkan::Result vulkanResult = this->vulkan->CreateDevice(deviceExtensionNames, 7);
    CELL_ASSERT(vulkanResult == Vulkan::Result::Success);

    const XrGraphicsBindingVulkanKHR vulkanBinding =
        {
            .type             = XR_TYPE_GRAPHICS_BINDING_VULKAN_KHR,
            .next             = nullptr,
            .instance         = this->vulkan->instance,
            .physicalDevice   = this->vulkan->physicalDevice,
            .device           = this->vulkan->device,
            .queueFamilyIndex = this->vulkan->physicalDeviceQueueGraphics,
            .queueIndex       = this->vulkan->physicalDeviceQueueGraphics,
        };

    const XrSessionCreateInfo sessionInfo =
        {
            .type        = XR_TYPE_SESSION_CREATE_INFO,
            .next        = &vulkanBinding,
            .createFlags = 0,
            .systemId    = this->systemId
        };

    xrResult = xrCreateSession(this->instance, &sessionInfo, &this->session);
    switch (xrResult) {
    case XR_SUCCESS: {
        break;
    }

    case XR_ERROR_RUNTIME_FAILURE:
    case XR_ERROR_INITIALIZATION_FAILED: {
        return Result::RuntimeFailure;
    }

    case XR_ERROR_INSTANCE_LOST: {
        return Result::RuntimeLost;
    }

    case XR_ERROR_OUT_OF_MEMORY: {
        return Result::OutOfMemory;
    }

    case XR_ERROR_LIMIT_REACHED: {
        return Result::LimitExceeded;
    }

    default: {
        System::Panic("xrCreateSession failed");
    }
    }

    return Result::Success;
}

}
