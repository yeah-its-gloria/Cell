// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Image.hh>

namespace Cell::Vulkan {

Image::~Image() {
    vkDestroySampler(this->device->device, this->sampler, nullptr);
    vkDestroyImageView(this->device->device, this->view, nullptr);
    vkDestroyImage(this->device->device, this->image, nullptr);
    vkFreeMemory(this->device->device, this->memory, nullptr);
}

}
