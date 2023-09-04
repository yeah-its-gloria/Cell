// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/CommandBufferManager.hh>

namespace Cell::Vulkan {

CommandBufferManager::~CommandBufferManager() {
    vkFreeCommandBuffers(this->instance->device, this->pool, this->buffers.GetCount(), &this->buffers);
    vkDestroyCommandPool(this->instance->device, this->pool, nullptr);
}

}
