// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Pipeline.hh>
#include <Cell/Vulkan/RenderTarget.hh>

namespace Cell::Vulkan {

Wrapped<Pipeline*, Result> Instance::CreatePipeline(IRenderTarget* target) {
    if (this->device == nullptr) {
        return Result::InvalidState;
    }

    return new Pipeline(this, target->GetColorFormat());
}

}
