// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/OpenXR/Instance.hh>

namespace Cell::OpenXR {

Wrapped<Instance::SessionPrepData, Result> Instance::PrepSession(const FormFactor formFactor) {
    const XrFormFactor factor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
    switch (formFactor) {
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
        .supportsHandTracking = XR_TRUE
    };

    XrSystemGetInfo systemGetInfo = {
        .type       = XR_TYPE_SYSTEM_GET_INFO,
        .next       = &systemHandProperties,
        .formFactor = factor
    };

    XrSystemId id = XR_NULL_SYSTEM_ID;
    XrResult result = xrGetSystem(this->instance, &systemGetInfo, &id);
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

    XrSystemProperties properties = { XR_TYPE_SYSTEM_PROPERTIES, nullptr, 0, 0, { }, { 0, 0, 0 }, { 0, 0 } };
    result = xrGetSystemProperties(this->instance, id, &properties);
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

    case XR_ERROR_OUT_OF_MEMORY: {
        return Result::OutOfMemory;
    }

    default: {
        System::Panic("xrGetSystemProperties failed");
    }
    }

    // View configuration retrieval

    uint32_t configurationTypeCount = 0;
    result = xrEnumerateViewConfigurations(this->instance, id, 0, &configurationTypeCount, nullptr);
    switch (result) {
    case XR_SUCCESS: {
        break;
    }

    default: {
        System::Panic("xrEnumerateViewConfigurations failed");
    }
    }

    Collection::List<XrViewConfigurationType> configurationsTypes(configurationTypeCount);
    result = xrEnumerateViewConfigurations(this->instance, id, configurationTypeCount, &configurationTypeCount, configurationsTypes.AsRaw());
    switch (result) {
    case XR_SUCCESS: {
        break;
    }

    default: {
        System::Panic("xrEnumerateViewConfigurations failed");
    }
    }

    bool typeFound = false;
    for (XrViewConfigurationType type : configurationsTypes) {
        if (type == XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO) {
            typeFound = true;
            break;
        }
    }

    if (!typeFound) {
        return Result::UnsupportedHardware;
    }

    XrViewConfigurationView viewConfigurations[2] = {
        { XR_TYPE_VIEW_CONFIGURATION_VIEW, nullptr, 0, 0, 0, 0, 0, 0 },
        { XR_TYPE_VIEW_CONFIGURATION_VIEW, nullptr, 0, 0, 0, 0, 0, 0 }
    };

    uint32_t count = 2;
    result = xrEnumerateViewConfigurationViews(this->instance, id, XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO, count, &count, viewConfigurations);
    switch (result) {
    case XR_SUCCESS: {
        break;
    }

    default: {
        System::Panic("xrEnumerateViewConfigurationViews failed");
    }
    }

    return SessionPrepData { id, properties, { viewConfigurations[0], viewConfigurations[1] } };
}

Wrapped<XrSpace, Result> Instance::PrepSessionSpace(XrSession session) {
    const XrPosef identityPose = {
        .orientation = { .x = 0.f, .y = 0.f, .z = 0.f, .w = 1.f },
        .position    = { .x = 0.f, .y = 0.f, .z = 0.f }
    };

    const XrReferenceSpaceCreateInfo spaceInfo = {
        .type                 = XR_TYPE_REFERENCE_SPACE_CREATE_INFO,
        .next                 = nullptr,

        .referenceSpaceType   = XR_REFERENCE_SPACE_TYPE_STAGE,
        .poseInReferenceSpace = identityPose
    };

    XrSpace space = nullptr;
    const XrResult result = xrCreateReferenceSpace(session, &spaceInfo, &space);
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

    case XR_ERROR_SESSION_LOST: {
        return Result::SessionLost;
    }

    case XR_ERROR_OUT_OF_MEMORY: {
        return Result::OutOfMemory;
    }

    case XR_ERROR_LIMIT_REACHED: {
        return Result::LimitExceeded;
    }

    case XR_ERROR_REFERENCE_SPACE_UNSUPPORTED: {
        return Result::Unavailable;
    }

    default: {
        System::Panic("xrCreateReferenceSpace failed");
    }
    }

    return space;
}

}
