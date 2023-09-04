// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Image.hh>

namespace Cell::Vulkan {

Instance::~Instance() {
    if (this->device != nullptr) {
        vkDeviceWaitIdle(this->device);

        vkDestroyDevice(this->device, nullptr);
    }

    vkDestroyInstance(this->instance, nullptr);
}

}
