// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Instance.hh>

namespace Cell::Vulkan {

Instance::~Instance() {
    vkDestroyInstance(this->instance, nullptr);
}

}
