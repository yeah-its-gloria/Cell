// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Device.hh>

namespace Cell::Vulkan {

Device::~Device() {
    vkDeviceWaitIdle(this->device);
    vkDestroyDevice(this->device, nullptr);
}

}
