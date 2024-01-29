// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/CommandBufferManager.hh>

namespace Cell::Vulkan {

CommandBufferManager::~CommandBufferManager() {
    vkFreeCommandBuffers(this->device->device, this->pool, this->buffers.GetCount(), this->buffers.AsRaw());
    vkDestroyCommandPool(this->device->device, this->pool, nullptr);
}

}
