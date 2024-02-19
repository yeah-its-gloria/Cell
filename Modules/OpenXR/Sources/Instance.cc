// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/OpenXR/Instance.hh>
#include <Cell/System/Panic.hh>

namespace Cell::OpenXR {

Wrapped<Instance*, Result> Instance::New() {
    const XrApplicationInfo applicationInfo = {
        .applicationName    = "Cell",
        .applicationVersion = 1,

        .engineName         = "Cell",
        .engineVersion      = 1,

        .apiVersion         = XR_MAKE_VERSION(1, 0, 27)
    };

    const char* extensions[] = {
#ifdef CELL_MODULES_VULKAN_AVAILABLE
        XR_KHR_VULKAN_ENABLE_EXTENSION_NAME,
#endif
#ifdef CELL_MODULES_D3D12_AVAILABLE
        XR_KHR_D3D12_ENABLE_EXTENSION_NAME,
#endif
        XR_EXT_HAND_TRACKING_EXTENSION_NAME,
        XR_FB_DISPLAY_REFRESH_RATE_EXTENSION_NAME,
        XR_KHR_COMPOSITION_LAYER_DEPTH_EXTENSION_NAME,
        XR_KHR_VISIBILITY_MASK_EXTENSION_NAME,
        XR_KHR_BINDING_MODIFICATION_EXTENSION_NAME
    };

    const XrInstanceCreateInfo instanceInfo = {
        .type                  = XR_TYPE_INSTANCE_CREATE_INFO,
        .next                  = nullptr,
        .createFlags           = 0,

        .applicationInfo       = applicationInfo,

        .enabledApiLayerCount  = 0,
        .enabledApiLayerNames  = nullptr,

        .enabledExtensionCount = sizeof(extensions) / sizeof(const char*),
        .enabledExtensionNames = extensions
    };

    XrInstance xrInstance = nullptr;
    const XrResult result = xrCreateInstance(&instanceInfo, &xrInstance);
    switch (result) {
    case XR_SUCCESS: {
        break;
    }

    case XR_ERROR_RUNTIME_FAILURE: {
        return Result::RuntimeFailure;
    }

    case XR_ERROR_OUT_OF_MEMORY: {
        return Result::OutOfMemory;
    }

    case XR_ERROR_EXTENSION_NOT_PRESENT: {
        return Result::FeatureMissing;
    }

    case XR_ERROR_API_LAYER_NOT_PRESENT: {
        return Result::DebugFeatureMissing;
    }

    case XR_ERROR_RUNTIME_UNAVAILABLE:
    case XR_ERROR_API_VERSION_UNSUPPORTED: {
        return Result::RuntimeUnavailable;
    }

    default: {
        System::Panic("xrCreateInstance failed");
    }
    }

    return new Instance(xrInstance);
}

Instance::~Instance() {
    XrResult result = XR_SUCCESS;

    result = xrDestroyInstance(this->instance);
    CELL_ASSERT(result == XR_SUCCESS);
}

}
