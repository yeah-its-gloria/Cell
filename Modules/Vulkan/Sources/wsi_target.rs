// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

mod format;
mod presentation;
mod properties;
mod swapchain;

use core::ptr::{null, null_mut};

use crate::ffi::vkDestroySwapchainKHR;

use self::{
    super::{
        ffi::{
            CreateFence, CreateSemaphore, DestroyFence, DestroySemaphore, DestroySurface, Extent2D, FenceCreateFlags, FenceCreateInfo, PresentMode, SemaphoreCreateInfo,
            StructureType, SurfaceCapabilities, SurfaceFormat, SurfaceKHR, SwapchainKHR, VkInstance, VkResult,
        },
        Device, Error, SynchronizationComponents,
    },
    format::HasFormatAndPresentMode,
    presentation::CanPresent,
};

/// Surface provider trait for implementors capable of generating a WSI compatible VkSurfaceKHR.
/// Mainly intended for Shell implementations.
pub trait SurfaceProvider {
    fn create_surface(&self, instance: VkInstance) -> Result<SurfaceKHR, Error>;
    fn get_extent(&self) -> (u32, u32);
}

/// Represents a presentable target.
pub struct WSITarget<'a> {
    parent: &'a Device<'a>,
    surface: SurfaceKHR,
    format: SurfaceFormat,
    mode: PresentMode,

    synchronization: [SynchronizationComponents; 4],

    capabilities: Option<SurfaceCapabilities>,
    extent: Option<Extent2D>,

    swapchain: Option<SwapchainKHR>,
}

impl Device<'_> {
    fn create_wsi_sync(&self) -> Result<[SynchronizationComponents; 4], Error> {
        let mut list = [SynchronizationComponents {
            image_available: null_mut(),
            render_finished: null_mut(),
            in_flight: null_mut(),
        }; 4];

        let semaphore_info = SemaphoreCreateInfo {
            s_type: StructureType::SemaphoreCreateInfo,
            next: null_mut(),
            flags: 0,
        };

        let fence_info = FenceCreateInfo {
            s_type: StructureType::FenceCreateInfo,
            next: null_mut(),
            flags: FenceCreateFlags::Signaled,
        };

        let mut result = VkResult::Success;
        for i in 0..list.len() {
            result = unsafe { CreateSemaphore(self.device, &semaphore_info, null(), &mut list[i].image_available) };
            if result != VkResult::Success {
                break;
            }

            result = unsafe { CreateSemaphore(self.device, &semaphore_info, null(), &mut list[i].render_finished) };
            if result != VkResult::Success {
                break;
            }

            result = unsafe { CreateFence(self.device, &fence_info, null(), &mut list[i].in_flight) };
            if result != VkResult::Success {
                break;
            }
        }

        match result {
            VkResult::Success => Ok(list),

            VkResult::ErrorOutOfHostMemory => {
                for c in list {
                    unsafe {
                        if !c.in_flight.is_null() {
                            DestroyFence(self.device, c.in_flight, null());
                        }

                        if !c.render_finished.is_null() {
                            DestroySemaphore(self.device, c.render_finished, null());
                        }

                        if !c.image_available.is_null() {
                            DestroySemaphore(self.device, c.image_available, null());
                        }
                    }
                }

                Err(Error::OutOfHostMemory)
            }

            VkResult::ErrorOutOfDeviceMemory => {
                for c in list {
                    unsafe {
                        if !c.in_flight.is_null() {
                            DestroyFence(self.device, c.in_flight, null());
                        }

                        if !c.render_finished.is_null() {
                            DestroySemaphore(self.device, c.render_finished, null());
                        }

                        if !c.image_available.is_null() {
                            DestroySemaphore(self.device, c.image_available, null());
                        }
                    }
                }

                Err(Error::OutOfDeviceMemory)
            }

            _ => {
                panic!("CreateSemaphore/CreateFence failed");
            }
        }
    }

    /// Creates a new WSITarget.
    /// This implements Vulkan's WSI VkSurfaceKHR and swapchains.
    pub fn create_wsi_target(&self, provider: &impl SurfaceProvider) -> Result<WSITarget, Error> {
        let surface = match provider.create_surface(self.parent.instance) {
            Ok(s) => s,
            Err(e) => return Err(e),
        };

        match surface.can_present_on_device(self.physical_device, self.graphics_index) {
            Ok(true) => {}

            Ok(false) => {
                unsafe { DestroySurface(self.parent.instance, surface, null()) };
                return Err(Error::ShellSessionNotUsable);
            }

            Err(e) => return Err(e),
        }

        let (format, mode) = match surface.retrieve_format_and_present_mode(self.physical_device) {
            Ok((format, mode)) => (format, mode),
            Err(e) => return Err(e),
        };

        let synchronization = match self.create_wsi_sync() {
            Ok(s) => s,
            Err(e) => return Err(e),
        };

        Ok(WSITarget {
            parent: self,
            surface,
            format,
            mode,
            synchronization,

            capabilities: None,
            extent: None,

            swapchain: None
        })
    }
}

impl WSITarget<'_> {
    // Sets up all resources required for rendering that aren't valid from creation until the target is dropped.
    // This includes the properties of the surface, the swapchain and its images.
    pub fn set_up_rendering(&mut self, provider: &impl SurfaceProvider) -> Result<(), Error> {
        match self.retrieve_properties(provider) {
            Ok((c, e)) => {
                self.capabilities = Some(c);
                self.extent = Some(e);
            }

            Err(e) => return Err(e),
        }

        match self.create_swapchain() {
            Ok(s) => {
                self.swapchain = Some(s);
            }

            Err(e) => return Err(e),
        }

        Ok(())
    }
}

impl Drop for WSITarget<'_> {
    fn drop(&mut self) {
        if self.swapchain.is_some() {
            unsafe { vkDestroySwapchainKHR(self.parent.device, self.swapchain.unwrap(), null()) };
        }

        for component in self.synchronization {
            unsafe {
                DestroyFence(self.parent.device, component.in_flight, null());
                DestroySemaphore(self.parent.device, component.render_finished, null());
                DestroySemaphore(self.parent.device, component.image_available, null());
            }
        }

        unsafe { DestroySurface(self.parent.parent.instance, self.surface, null_mut()) };
    }
}
