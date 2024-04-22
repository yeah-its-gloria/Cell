// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/OpenXR/Session.hh>

namespace Cell::OpenXR {

Wrapped<Session*, Result> Instance::CreateSessionD3D12(const FormFactor formFactor, Renderer::D3D12::Device* device) {
    Wrapped<SessionPrepData, Result> prepDataResult = this->PrepSession(formFactor);
    if (!prepDataResult.IsValid()) {
        return prepDataResult.Result();
    }

    SessionPrepData prepData = prepDataResult.Unwrap();

    PFN_xrGetD3D12GraphicsRequirementsKHR requirementsFunc = nullptr;
    XrResult result = xrGetInstanceProcAddr(this->instance, "xrGetD3D12GraphicsRequirementsKHR", (PFN_xrVoidFunction*)&requirementsFunc);
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

    XrGraphicsRequirementsD3D12KHR requirements = {
        .type            = XR_TYPE_GRAPHICS_REQUIREMENTS_D3D12_KHR,
        .next            = nullptr,
        .adapterLuid     = { 0, 0 },
        .minFeatureLevel = D3D_FEATURE_LEVEL_12_0,
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
        System::Panic("xrGetD3D12GraphicsRequirementsKHR failed");
    }
    }

    if (requirements.minFeatureLevel > D3D_FEATURE_LEVEL_12_0) {
        return Result::UnsupportedHardware;
    }

    const XrGraphicsBindingD3D12KHR binding = {
        .type   = XR_TYPE_GRAPHICS_BINDING_D3D12_KHR,
        .next   = nullptr,

        .device = device->device,
        .queue  = device->mainQueue
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
