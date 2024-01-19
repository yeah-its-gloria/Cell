// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Image.hh>

namespace Cell::Vulkan {

Image::~Image() {
    vkDestroySampler(this->instance->device, this->sampler, nullptr);
    vkDestroyImageView(this->instance->device, this->view, nullptr);
    vkDestroyImage(this->instance->device, this->image, nullptr);
    vkFreeMemory(this->instance->device, this->memory, nullptr);
}

}
