// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/OpenXR/Instance.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Panic.hh>

#include <openxr/openxr_reflection.h>

#define XR_ENUM_CASE_STR(name, val) case name: return #name;
#define XR_ENUM_STR(enumType) constexpr const char* XrEnumStr(enumType e) { switch (e) { XR_LIST_ENUM_##enumType(XR_ENUM_CASE_STR) default: return "Unknown"; } }

XR_ENUM_STR(XrStructureType);

XR_ENUM_STR(XrSessionState);

namespace Cell::OpenXR {

Result Instance::UpdateState() {
    XrEventDataBuffer event = {
        .type    = XR_TYPE_EVENT_DATA_BUFFER,
        .next    = nullptr,
        .varying = { 0 }
    };

    XrResult result = xrPollEvent(this->instance, &event);
    switch (result) {
    case XR_SUCCESS: {
        break;
    }

    case XR_EVENT_UNAVAILABLE: {
        return Result::Unavailable;
    }

    case XR_ERROR_RUNTIME_FAILURE: {
        return Result::RuntimeFailure;
    }

    case XR_ERROR_INSTANCE_LOST: {
        return Result::RuntimeLost;
    }

    default: {
        System::Panic("xrPollEvent failed");
    }
    }

    switch (event.type) {
    case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED: {
        XrEventDataSessionStateChanged* sessionChange = (XrEventDataSessionStateChanged*)&event;
        this->state = sessionChange->state;
        break;
    }

    case XR_TYPE_EVENT_DATA_REFERENCE_SPACE_CHANGE_PENDING: {
        XrEventDataReferenceSpaceChangePending* change = (XrEventDataReferenceSpaceChangePending*)&event;
        System::Log("Changing space: %d", change->referenceSpaceType);

        // TODO: handle this
        break;
    }

    default: {
        System::Panic("Invalid event type %s", XrEnumStr(event.type));
    }
    }

    return Result::Success;
}

}
