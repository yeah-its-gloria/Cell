// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/List.hh>
#include <Cell/OpenXR/Instance.hh>

namespace Cell::OpenXR {

Result Instance::FetchViewConfiguration() {
    if (this->systemId == 0) {
        return Result::InvalidState;
    }

    uint32_t configurationTypeCount = 0;
    XrResult result = xrEnumerateViewConfigurations(this->instance, this->systemId, 0, &configurationTypeCount, nullptr);
    switch (result) {
    case XR_SUCCESS: {
        break;
    }

    default: {
        System::Panic("xrEnumerateViewConfigurations failed");
    }
    }

    List<XrViewConfigurationType> configurationsTypes(configurationTypeCount);
    result = xrEnumerateViewConfigurations(this->instance, this->systemId, configurationTypeCount, &configurationTypeCount, &configurationsTypes);
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

    uint32_t count = 2;
    result = xrEnumerateViewConfigurationViews(this->instance, this->systemId, XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO, count, &count, this->viewConfigs);
    switch (result) {
    case XR_SUCCESS: {
        break;
    }

    default: {
        System::Panic("xrEnumerateViewConfigurationViews failed");
    }
    }

    return Result::Success;
}

}
