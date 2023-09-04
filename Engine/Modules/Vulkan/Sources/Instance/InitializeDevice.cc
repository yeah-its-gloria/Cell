// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Instance.hh>

namespace Cell::Vulkan {

#define CHECK_RETURN if (result != Result::Success) { return result; }

Result Instance::InitializeDevice() {
    if (this->device != nullptr) {
        return Result::InvalidState;
    }

    Result result = this->QueryPhysicalDevice();
    CHECK_RETURN

    result = this->CreateDevice();
    CHECK_RETURN

    return Result::Success;
}

}
