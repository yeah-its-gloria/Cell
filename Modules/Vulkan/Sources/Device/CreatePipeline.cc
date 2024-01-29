// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Vulkan/Pipeline.hh>
#include <Cell/Vulkan/RenderTarget.hh>

namespace Cell::Vulkan {

Wrapped<Pipeline*, Result> Device::CreatePipeline(IRenderTarget* target) {
    return new Pipeline(this, target->GetColorFormat());
}

}
