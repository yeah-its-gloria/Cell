// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/OpenXR/Session.hh>

namespace Cell::OpenXR {

Wrapped<Session*, Result> Instance::CreateSessionVulkan(const FormFactor formFactor, Renderer::Vulkan::Device* device) {
    Wrapped<SessionPrepData, Result> prepDataResult = this->PrepSession(formFactor);
    if (!prepDataResult.IsValid()) {
        return prepDataResult.Result();
    }

    SessionPrepData prepData = prepDataResult.Unwrap();

    PFN_xrGetVulkanGraphicsRequirementsKHR requirementsFunc = nullptr;
    XrResult result = xrGetInstanceProcAddr(this->instance, "xrGetVulkanGraphicsRequirementsKHR", (PFN_xrVoidFunction*)&requirementsFunc);
    switch (result) {
    case XR_SUCCESS: {
        break;
    }

    case XR_ERROR_RUNTIME_FAILURE:
    case XR_ERROR_FUNCTION_UNSUPPORTED: {
        return Result::RuntimeFailure;
    }

    case XR_ERROR_INSTANCE_LOST: {
        return Result::RuntimeLost;
    }

    case XR_ERROR_OUT_OF_MEMORY: {
        return Result::OutOfMemory;
    }

    default: {
        System::Panic("xrGetInstanceProcAddr failed");
    }
    }

    XrGraphicsRequirementsVulkanKHR requirements = {
        .type                   = XR_TYPE_GRAPHICS_REQUIREMENTS_VULKAN_KHR,
        .next                   = nullptr,
        .minApiVersionSupported = 0,
        .maxApiVersionSupported = 0
    };

    result = requirementsFunc(this->instance, prepData.id, &requirements);
    switch (result) {
    case XR_SUCCESS: {
        break;
    }

    case XR_ERROR_RUNTIME_FAILURE:
    case XR_ERROR_FUNCTION_UNSUPPORTED:
    case XR_ERROR_SYSTEM_INVALID: {
        return Result::RuntimeFailure;
    }

    case XR_ERROR_INSTANCE_LOST: {
        return Result::RuntimeLost;
    }

    default: {
        System::Panic("xrGetVulkanGraphicsRequirementsKHR failed");
    }
    }

    if (XR_VERSION_MAJOR(requirements.minApiVersionSupported) > 1 || XR_VERSION_MINOR(requirements.minApiVersionSupported) > 2 ||
        XR_VERSION_MAJOR(requirements.maxApiVersionSupported) < 1 || XR_VERSION_MINOR(requirements.maxApiVersionSupported) < 2) {
        return Result::UnsupportedHardware;
    }

    const XrGraphicsBindingVulkanKHR binding = {
        .type             = XR_TYPE_GRAPHICS_BINDING_VULKAN_KHR,
        .next             = nullptr,

        .instance         = device->instance->instance,

        .physicalDevice   = device->physicalDevice,
        .device           = device->device,

        .queueFamilyIndex = device->physicalDeviceQueueGraphics,
        .queueIndex       = 0
    };

    const XrSessionCreateInfo sessionInfo = {
        .type        = XR_TYPE_SESSION_CREATE_INFO,
        .next        = &binding,
        .createFlags = 0,
        .systemId    = prepData.id
    };

    XrSession session = nullptr;
    result = xrCreateSession(this->instance, &sessionInfo, &session);
    switch (result) {
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

    Wrapped<XrSpace, Result> spaceResult = this->PrepSessionSpace(session);
    if (!prepDataResult.IsValid()) {
        xrDestroySession(session);
        return prepDataResult.Result();
    }

    return new Session(this, prepData.id, prepData.properties, prepData.viewConfigurations, session, spaceResult.Unwrap());
}

}
