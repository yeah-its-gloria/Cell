// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/OpenXR/VulkanTarget.hh>

namespace Cell::OpenXR {

Result Instance::CreateRenderTargetsVulkan() {
    if (this->session == nullptr) {
        return Result::InvalidState;
    }

    Result result = Result::Success;
    for (uint8_t i = 0; i < ViewCount; i++) {
        this->vulkanTargets[i] = new VulkanTarget(this, &this->viewConfigs[i], &this->views[i]);

        result = this->vulkanTargets[i]->CreateSwapchains();
        if (result != Result::Success) {
            break;
        }
    }

    return result;
}

}
