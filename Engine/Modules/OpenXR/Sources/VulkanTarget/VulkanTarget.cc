// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/OpenXR/VulkanTarget.hh>

namespace Cell::OpenXR
{

VulkanTarget::~VulkanTarget()
{
    for (VkImageView view : swapchainDepthImageViews)
    {
        vkDestroyImageView(this->instance->GetVulkan()->device, view, nullptr);
    }

    for (VkImageView view : swapchainColorImageViews)
    {
        vkDestroyImageView(this->instance->GetVulkan()->device, view, nullptr);
    }

    //xrDestroySwapchain(depthSwapchain);
    //xrDestroySwapchain(colorSwapchain);
}

}
