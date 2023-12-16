// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use super::super::{
    ffi::{GetSurfaceSupport, PhysicalDevice, SurfaceKHR, VkResult, VK_FALSE, VK_TRUE},
    Error,
};

pub trait CanPresent {
    fn can_present_on_device(&self, device: PhysicalDevice, queue_family_index: u32) -> Result<bool, Error>;
}

impl CanPresent for SurfaceKHR {
    fn can_present_on_device(&self, device: PhysicalDevice, queue_family_index: u32) -> Result<bool, Error> {
        let mut can_present = VK_FALSE;
        match unsafe { GetSurfaceSupport(device, queue_family_index, *self, &mut can_present) } {
            VkResult::Success => Ok(can_present == VK_TRUE),
            VkResult::ErrorOutOfHostMemory => Err(Error::OutOfHostMemory),
            VkResult::ErrorOutOfDeviceMemory => Err(Error::OutOfDeviceMemory),
            VkResult::ErrorSurfaceLost => Err(Error::SurfaceLost),

            _ => {
                panic!("GetSurfaceSupport failed")
            }
        }
    }
}
