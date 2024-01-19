// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/OpenXR/VulkanTarget.hh>
#include <Cell/System/Panic.hh>

namespace Cell::OpenXR {

Instance::~Instance() {
    XrResult result = XR_SUCCESS;

    if (this->session != nullptr) {
        result = xrDestroySession(this->session);
        CELL_ASSERT(result == XR_SUCCESS);
    }

    for (uint8_t i = 0; i < 2; i++) {
        delete this->vulkanTargets[i];
    }

    result = xrDestroyInstance(this->instance);
    CELL_ASSERT(result == XR_SUCCESS);
}

}
