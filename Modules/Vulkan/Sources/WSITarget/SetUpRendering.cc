// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/WSITarget.hh>

namespace Cell::Vulkan {

#define CHECK_RETURN if (result != Result::Success) { return result; }

Result WSITarget::SetUpRendering() {
    Result result = this->RetrieveProperties();
    CHECK_RETURN

    result = this->CreateSwapchain();
    CHECK_RETURN

    result = this->PrepareSwapchain();
    CHECK_RETURN

    result = this->SetUpDepthBuffer();
    CHECK_RETURN

    return this->SetUpSynchronization();
}

}
