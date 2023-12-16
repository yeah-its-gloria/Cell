// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use super::{
    super::{
        ffi::{Extent2D, GetSurfaceCapabilities, SurfaceCapabilities, VkResult},
        Error,
    },
    SurfaceProvider, WSITarget,
};

impl WSITarget<'_> {
    pub fn retrieve_properties(&self, provider: &impl SurfaceProvider) -> Result<(SurfaceCapabilities, Extent2D), Error> {
        let mut capabilities = SurfaceCapabilities::default();
        match unsafe { GetSurfaceCapabilities(self.parent.physical_device, self.surface, &mut capabilities) } {
            VkResult::Success => {}

            VkResult::ErrorOutOfHostMemory => return Err(Error::OutOfHostMemory),
            VkResult::ErrorOutOfDeviceMemory => return Err(Error::OutOfDeviceMemory),
            VkResult::ErrorSurfaceLost => return Err(Error::SurfaceLost),

            _ => {
                panic!("GetSurfaceCapabilities failed");
            }
        }

        let extent = if capabilities.current_extent.width != u32::MAX && capabilities.current_extent.height != u32::MAX {
            capabilities.current_extent
        } else {
            let shell_extent = provider.get_extent();

            Extent2D {
                width: shell_extent.0.min(capabilities.min_image_extent.width).max(capabilities.max_image_extent.width),
                height: shell_extent.1.min(capabilities.min_image_extent.height).max(capabilities.max_image_extent.height),
            }
        };

        Ok((capabilities, extent))
    }
}
