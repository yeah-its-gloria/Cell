// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/OpenXR/Instance.hh>
#include <Cell/System/Panic.hh>

namespace Cell::OpenXR {

Result Instance::BeginSession() {
    const XrSessionBeginInfo beginInfo = {
        .type                         = XR_TYPE_SESSION_BEGIN_INFO,
        .next                         = nullptr,
        .primaryViewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO
    };

    const XrResult result = xrBeginSession(this->session, &beginInfo);
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

    case XR_ERROR_SESSION_RUNNING: {
        return Result::AlreadyActive;
    }

    case XR_ERROR_SESSION_NOT_READY: {
        return Result::NotYetReady;
    }

    default: {
        System::Panic("xrBeginSession failed");
    }
    }

    return Result::Success;
}

Result Instance::EndSession() {
    const XrResult result = xrEndSession(this->session);
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

    case XR_ERROR_SESSION_NOT_STOPPING: {
        return Result::NotYetReady;
    }

    case XR_ERROR_SESSION_NOT_RUNNING: {
        return Result::NotActive;
    }

    default: {
        System::Panic("xrEndSession failed");
    }
    }

    return Result::Success;
}

Result Instance::RequestExit() {
    XrResult result = xrRequestExitSession(this->session);
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
        System::Panic("xrRequestExitSession failed");
    }
    }

    return Result::Success;
}

SessionState Instance::GetSessionState() {
    switch (this->state) {
    case XR_SESSION_STATE_IDLE: {
        return SessionState::Idle;
    }

    case XR_SESSION_STATE_READY: {
        return SessionState::Ready;
    }

    case XR_SESSION_STATE_STOPPING: {
        return SessionState::Stopping;
    }

    case XR_SESSION_STATE_SYNCHRONIZED: {
        return SessionState::Synchronized;
    }

    case XR_SESSION_STATE_VISIBLE: {
        return SessionState::Visible;
    }

    case XR_SESSION_STATE_FOCUSED: {
        return SessionState::Focussed;
    }

    case XR_SESSION_STATE_LOSS_PENDING: {
        return SessionState::Ending;
    }

    case XR_SESSION_STATE_EXITING: {
        return SessionState::Exiting;
    }

    default: {
        break;
    }
    }

    CELL_UNREACHABLE;
}


}
