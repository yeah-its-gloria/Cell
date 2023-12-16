// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use core::ptr::{null, null_mut};

use crate::{
    ffi::{vkCreateSwapchainKHR, CompositeAlphaFlag, SharingMode, StructureType, SwapchainCreateInfo, SwapchainKHR, VkResult, VK_TRUE, vkGetSwapchainImagesKHR, Image, ImageView},
    wsi_target::WSITarget,
    Error,
};

impl WSITarget<'_> {
    pub fn create_swapchain(&self) -> Result<SwapchainKHR, Error> {
        let info = SwapchainCreateInfo {
            s_type: StructureType::SwapchainCreateInfo,
            next: null_mut(),
            flags: 0,

            surface: self.surface,

            min_image_count: 4,

            format: self.format.format,
            color_space: self.format.color_space,
            extent: self.extent.unwrap(),
            array_layers: 1,
            usage: 0,
            sharing_mode: SharingMode::Exclusive,

            queue_family_index_count: 0,
            queue_family_indices: null(),

            pre_transform: self.capabilities.as_ref().unwrap().current_transform,
            composite_alpha: CompositeAlphaFlag::Opaque,
            present_mode: self.mode,

            clipped: VK_TRUE,

            old_swapchain: null_mut(),
        };

        let mut swapchain: SwapchainKHR = null_mut();
        match unsafe { vkCreateSwapchainKHR(self.parent.device, &info, null(), &mut swapchain) } {
            VkResult::Success => {}

            VkResult::ErrorOutOfHostMemory => return Err(Error::OutOfHostMemory),
            VkResult::ErrorOutOfDeviceMemory => return Err(Error::OutOfDeviceMemory),
            VkResult::ErrorDeviceLost => return Err(Error::DeviceLost),
            VkResult::ErrorSurfaceLost => return Err(Error::SurfaceLost),

            _ => {
                panic!("vkCreateSwapchainKHR failed");
            }
        }

        let mut image_count = 4;
        let mut images: [Image; 4] = [null_mut(); 4];
        match unsafe { vkGetSwapchainImagesKHR(self.parent.device, swapchain, &mut image_count, images.as_mut_ptr()) } {
            VkResult::Success => {}

            VkResult::ErrorOutOfHostMemory => return Err(Error::OutOfHostMemory),
            VkResult::ErrorOutOfDeviceMemory => return Err(Error::OutOfDeviceMemory),

            _ => {
                panic!("vkGetSwapchainImagesKHR failed");
            }
        }

        let mut _image_views: [ImageView; 4] = [null_mut(); 4];
        for _image in images {
            // ...
        }

        Ok(swapchain)
    }
}
