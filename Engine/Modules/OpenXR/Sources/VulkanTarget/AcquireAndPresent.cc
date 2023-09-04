// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/OpenXR/VulkanTarget.hh>
#include <Cell/System/Panic.hh>

namespace Cell::OpenXR
{

CELL_INLINE Vulkan::Result convertResult(XrResult result, const char* functionName)
{
    switch (result)
    {
    case XR_SUCCESS:
    {
        break;
    }

    case XR_ERROR_RUNTIME_FAILURE:
    {
        return Vulkan::Result::InstanceFailure;
    }

    case XR_ERROR_INSTANCE_LOST:
    {
        return Vulkan::Result::DeviceLost;
    }

    case XR_ERROR_SESSION_LOST:
    {
        return Vulkan::Result::SurfaceLost;
    }

    default:
    {
        System::Panic("%s failed", functionName);
    }
    }

    return Vulkan::Result::Success;
}

Wrapped<Vulkan::AcquiredImage, Vulkan::Result> VulkanTarget::AcquireNext()
{
    const XrSwapchainImageAcquireInfo acquireInfo =
    {
        .type = XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO,
        .next = nullptr
    };

    const XrSwapchainImageWaitInfo imageWaitInfo =
    {
        .type    = XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO,
        .next    = nullptr,
        .timeout = 1000
    };

    uint32_t colorAcquiredIndex = 0;
    uint32_t depthAcquiredIndex = 0;

    XrResult result = xrAcquireSwapchainImage(this->colorSwapchain, &acquireInfo, &colorAcquiredIndex);
    Vulkan::Result vkResult = convertResult(result, "xrAcquireSwapchainImage");
    if (vkResult != Vulkan::Result::Success)
    {
        return vkResult;
    }

    result = xrWaitSwapchainImage(this->colorSwapchain, &imageWaitInfo);
    vkResult = convertResult(result, "xrWaitSwapchainImage");
    if (vkResult != Vulkan::Result::Success)
    {
        return vkResult;
    }

    result = xrAcquireSwapchainImage(this->depthSwapchain, &acquireInfo, &depthAcquiredIndex);
    vkResult = convertResult(result, "xrAcquireSwapchainImage");
    if (vkResult != Vulkan::Result::Success)
    {
        return vkResult;
    }

    result = xrWaitSwapchainImage(this->depthSwapchain, &imageWaitInfo);
    vkResult = convertResult(result, "xrWaitSwapchainImage");
    if (vkResult != Vulkan::Result::Success)
    {
        return vkResult;
    }

    CELL_ASSERT(colorAcquiredIndex == depthAcquiredIndex);

    this->projectionView.pose = this->view->pose;
    this->projectionView.fov  = this->view->fov;

    this->imageIndex = colorAcquiredIndex;

    return Vulkan::AcquiredImage
    {
        .image     = this->swapchainColorImages[this->imageIndex].image,
        .usesSync  = false,
        .available = nullptr,
        .rendered  = nullptr,
        .inFlight  = nullptr
    };
}

Vulkan::Result VulkanTarget::Present()
{
    const XrSwapchainImageReleaseInfo releaseInfo =
    {
        .type = XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO,
        .next = nullptr
    };

    XrResult result = xrReleaseSwapchainImage(this->colorSwapchain, &releaseInfo);
    Vulkan::Result vkResult = convertResult(result, "xrReleaseSwapchainImage");
    if (vkResult != Vulkan::Result::Success)
    {
        return vkResult;
    }

    result = xrReleaseSwapchainImage(this->depthSwapchain, &releaseInfo);
    return convertResult(result, "xrReleaseSwapchainImage");
}

}
