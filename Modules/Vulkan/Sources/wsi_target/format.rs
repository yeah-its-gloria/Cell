// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use alloc::{vec, vec::Vec};
use core::ptr::null_mut;

use super::super::{
    ffi::{ColorSpace, Format, GetSurfaceFormats, GetSurfacePresentModes, PhysicalDevice, PresentMode, SurfaceFormat, SurfaceKHR, VkResult},
    Error,
};

pub trait HasFormatAndPresentMode {
    fn retrieve_format_and_present_mode(&self, device: PhysicalDevice) -> Result<(SurfaceFormat, PresentMode), Error>;
}

impl HasFormatAndPresentMode for SurfaceKHR {
    fn retrieve_format_and_present_mode(&self, device: PhysicalDevice) -> Result<(SurfaceFormat, PresentMode), Error> {
        let mut format_count: u32 = 0;
        match unsafe { GetSurfaceFormats(device, *self, &mut format_count, null_mut()) } {
            VkResult::Success => {}

            VkResult::ErrorOutOfHostMemory => return Err(Error::OutOfHostMemory),
            VkResult::ErrorOutOfDeviceMemory => return Err(Error::OutOfDeviceMemory),

            _ => {
                panic!("GetSurfaceFormats failed");
            }
        }

        assert!(format_count > 0);

        let mut mode_count: u32 = 0;
        match unsafe { GetSurfacePresentModes(device, *self, &mut mode_count, null_mut()) } {
            VkResult::Success => {}

            VkResult::ErrorOutOfHostMemory => return Err(Error::OutOfHostMemory),
            VkResult::ErrorOutOfDeviceMemory => return Err(Error::OutOfDeviceMemory),

            _ => {
                panic!("GetSurfacePresentModes failed");
            }
        }

        assert!(mode_count > 0);

        let mut formats: Vec<SurfaceFormat> = vec![
            SurfaceFormat {
                format: Format::Undefined,
                color_space: ColorSpace::sRGB_Nonlinear
            };
            format_count as usize
        ];

        let mut modes: Vec<PresentMode> = vec![PresentMode::Immediate; format_count as usize];

        match unsafe { GetSurfaceFormats(device, *self, &mut format_count, formats.as_mut_ptr()) } {
            VkResult::Success => {}

            VkResult::ErrorOutOfHostMemory => return Err(Error::OutOfHostMemory),
            VkResult::ErrorOutOfDeviceMemory => return Err(Error::OutOfDeviceMemory),

            _ => {
                panic!("GetSurfaceFormats failed");
            }
        }

        match unsafe { GetSurfacePresentModes(device, *self, &mut mode_count, modes.as_mut_ptr()) } {
            VkResult::Success => {}

            VkResult::ErrorOutOfHostMemory => return Err(Error::OutOfHostMemory),
            VkResult::ErrorOutOfDeviceMemory => return Err(Error::OutOfDeviceMemory),

            _ => {
                panic!("GetSurfacePresentModes failed");
            }
        }

        let format = formats
            .iter()
            .find(|a| a.format == Format::R8G8B8A8_sRGB && a.color_space == ColorSpace::sRGB_Nonlinear)
            .unwrap_or(&formats[0])
            .clone();

        let mode = if modes.iter().any(|x| *x == PresentMode::Mailbox) {
            PresentMode::Mailbox
        } else {
            PresentMode::Immediate
        };

        Ok((format, mode))
    }
}
