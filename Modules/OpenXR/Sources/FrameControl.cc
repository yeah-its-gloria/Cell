// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/OpenXR/VulkanTarget.hh>

namespace Cell::OpenXR {

Result Instance::BeginFrame() {
    if (this->state != XR_SESSION_STATE_READY &&
        this->state != XR_SESSION_STATE_SYNCHRONIZED &&
        this->state != XR_SESSION_STATE_VISIBLE &&
        this->state != XR_SESSION_STATE_FOCUSED) {
        return Result::InvalidState;
    }

    XrResult result = xrWaitFrame(this->session, nullptr, &this->frameState);
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

    case XR_ERROR_SESSION_NOT_RUNNING: {
        return Result::NotActive;
    }

    default: {
        System::Panic("xrWaitFrame failed");
    }
    }

    if (this->frameState.shouldRender == XR_FALSE) {
        return Result::NotYetReady;
    }

    // locate view

    const XrViewLocateInfo viewLocateInfo = {
        .type                  = XR_TYPE_VIEW_LOCATE_INFO,
        .next                  = nullptr,
        .viewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO,
        .displayTime           = this->frameState.predictedDisplayTime,
        .space                 = this->space
    };

    XrViewState viewState = {
        .type           = XR_TYPE_VIEW_STATE,
        .next           = nullptr,
        .viewStateFlags = 0
    };

    uint32_t viewCount = ViewCount;
    result = xrLocateViews(this->session, &viewLocateInfo, &viewState, viewCount, &viewCount, this->views);
    switch (result) {
    case XR_SUCCESS: {
        break;
    }

    default: {
        System::Panic("xrAcquireSwapchainImage failed");
    }
    }

    if ((viewState.viewStateFlags & (XR_VIEW_STATE_ORIENTATION_VALID_BIT)) != XR_VIEW_STATE_ORIENTATION_VALID_BIT) {
        return Result::ViewOrPoseMalformed;
    }

    // start rendering

    const XrFrameBeginInfo beginInfo =
        {
            .type = XR_TYPE_FRAME_BEGIN_INFO,
            .next = nullptr
        };

    result = xrBeginFrame(this->session, &beginInfo);
    switch (result) {
    case XR_SUCCESS:
    case XR_FRAME_DISCARDED: {
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

    default: {
        System::Panic("xrBeginFrame failed");
    }
    }

    return Result::Success;
}

Result Instance::EndFrame() {
    XrCompositionLayerProjectionView projectionViews[ViewCount] = {
        this->vulkanTargets[0]->projectionView,
        this->vulkanTargets[1]->projectionView
    };

    const XrCompositionLayerProjection projectionLayer = {
        .type       = XR_TYPE_COMPOSITION_LAYER_PROJECTION,
        .next       = nullptr,
        .layerFlags = 0,
        .space      = this->space,
        .viewCount  = ViewCount,
        .views      = projectionViews
    };

    const XrCompositionLayerBaseHeader* layers[1] = {
        (const XrCompositionLayerBaseHeader*)&projectionLayer
    };

    const XrFrameEndInfo endInfo = {
        .type                 = XR_TYPE_FRAME_END_INFO,
        .next                 = nullptr,
        .displayTime          = this->frameState.predictedDisplayTime,
        .environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE,
        .layerCount           = 1,
        .layers               = layers
    };

    const XrResult result = xrEndFrame(this->session, &endInfo);
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

    default: {
        System::Panic("xrEndFrame failed");
    }
    }

    return Result::Success;
}

}
