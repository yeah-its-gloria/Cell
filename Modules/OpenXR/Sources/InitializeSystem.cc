// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/OpenXR/Instance.hh>
#include <Cell/System/Panic.hh>

namespace Cell::OpenXR {

Result Instance::InitializeSystem(FormFactor factor) {
    const XrFormFactor formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
    switch (factor) {
    case FormFactor::HeadMountedDisplay: {
        break;
    }

    default: {
        return Result::InvalidParameters;
    }
    }

    XrSystemHandTrackingPropertiesEXT systemHandProperties = {
        .type                 = XR_TYPE_SYSTEM_HAND_TRACKING_PROPERTIES_EXT,
        .next                 = nullptr,
        .supportsHandTracking = XR_FALSE
    };

    XrSystemGetInfo systemGetInfo = {
        .type       = XR_TYPE_SYSTEM_GET_INFO,
        .next       = &systemHandProperties,
        .formFactor = formFactor
    };

    XrResult result = xrGetSystem(this->instance, &systemGetInfo, &this->systemId);
    switch (result) {
    case XR_SUCCESS: {
        break;
    }

    case XR_ERROR_RUNTIME_FAILURE: {
        return Result::RuntimeFailure;
    }

    case XR_ERROR_INSTANCE_LOST: {
        return Result::RuntimeLost;
    }

    case XR_ERROR_FORM_FACTOR_UNSUPPORTED: {
        return Result::UnsupportedHardware;
    }

    case XR_ERROR_FORM_FACTOR_UNAVAILABLE: {
        return Result::Unavailable;
    }

    default: {
        System::Panic("xrGetSystem failed");
    }
    }

    result = xrGetSystemProperties(this->instance, this->systemId, &this->properties);
    switch (result) {
    case XR_SUCCESS: {
        break;
    }

    case XR_ERROR_RUNTIME_FAILURE: {
        this->systemId = 0;
        return Result::RuntimeFailure;
    }

    case XR_ERROR_INSTANCE_LOST: {
        this->systemId = 0;
        return Result::RuntimeLost;
    }

    case XR_ERROR_OUT_OF_MEMORY: {
        this->systemId = 0;
        return Result::OutOfMemory;
    }

    default: {
        System::Panic("xrGetSystemProperties failed");
    }
    }

    return Result::Success;
}

}
