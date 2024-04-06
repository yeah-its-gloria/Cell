// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use core::ptr::{null, null_mut};

use crate::{
    ffi::{
        vkCreateImage, vkDestroyImage, vkGetImageMemoryRequirements, Extent3D, Format, ImageCreateInfo, ImageLayout, ImageType, ImageUsageFlags, MemoryRequirements,
        SampleCount, SharingMode, StructureType, TilingMode, VkImage, VkResult,
    },
    Device, Error,
};

/// Represents a device instance.
pub struct Image<'a> {
    parent: &'a Device<'a>,
    image: VkImage,
}

// const uint32_t width, const uint32_t height, const VkFormat format,
// const VkImageTiling tiling, const VkImageAspectFlags viewAspectMask, const VkImageUsageFlags usage

impl Device<'_> {
    /// Creates a new image.
    pub fn create_image(&self, width: u32, height: u32) -> Result<Image, Error> {
        let info = ImageCreateInfo {
            s_type: StructureType::ImageCreateInfo,
            next: null_mut(),
            flags: 0,

            image_type: ImageType::Image2D,
            format: Format::R8G8B8A8_sRGB,

            extent: Extent3D { width, height, depth: 1 },

            mip_levels: 1,
            array_layers: 1,
            samples: SampleCount::OneBit,

            tiling: TilingMode::Optimal,
            usage: ImageUsageFlags::AsTransferDestination,
            sharing_mode: SharingMode::Exclusive,

            queue_family_index_count: 0,
            queue_family_indices: null(),

            initial_layout: ImageLayout::Undefined,
        };

        let mut image: VkImage = null_mut();
        match unsafe { vkCreateImage(self.device, &info, null(), &mut image) } {
            VkResult::Success => {}

            VkResult::ErrorOutOfHostMemory => return Err(Error::OutOfHostMemory),
            VkResult::ErrorOutOfDeviceMemory => return Err(Error::OutOfDeviceMemory),

            _ => {
                panic!("CreateImage failed");
            }
        }

        let mut requirements = MemoryRequirements::default();
        unsafe { vkGetImageMemoryRequirements(self.device, image, &mut requirements) };

        // TODO: allocate memory, bind to image, etc.

        Ok(Image { parent: self, image })
    }
}

impl Drop for Image<'_> {
    fn drop(&mut self) {
        unsafe { vkDestroyImage(self.parent.device, self.image, null()) };
    }
}
