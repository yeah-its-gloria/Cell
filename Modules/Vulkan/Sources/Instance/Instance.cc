// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Instance.hh>

namespace Cell::Vulkan {

Instance::~Instance() {
    if (this->device != nullptr) {
        vkDeviceWaitIdle(this->device);

        vkDestroyDevice(this->device, nullptr);
    }

    vkDestroyInstance(this->instance, nullptr);
}

}
