// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/OpenXR/Instance.hh>

namespace Cell::OpenXR {

Result Instance::CreateSpace() {
    if (this->session == nullptr) {
        return Result::InvalidState;
    }

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

    XrResult result = xrCreateReferenceSpace(this->session, &spaceInfo, &this->space);
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

    return Result::Success;
}

}
