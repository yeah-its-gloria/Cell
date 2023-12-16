// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

mod features;
mod queue_families;
mod score;

use alloc::{vec, vec::Vec};
use core::{
    ffi::c_void,
    ptr::{null, null_mut},
};

use self::{
    super::{
        extension_list,
        ffi::{
            vkEnumeratePhysicalDevices, CreateDevice, DestroyDevice, DeviceCreateInfo, DeviceQueueCreateInfo, GetDeviceQueue, PhysicalDevice,
            PhysicalDeviceDynamicRenderingFeatures, PhysicalDeviceExtendedDynamicStateFeatures, Queue, StructureType, VkDevice, VkResult, WaitForDeviceIdle, VK_TRUE,
        },
        Error, Instance,
    },
    features::DEVICE_FEATURES,
    queue_families::HasFamilyQueueIndices,
    score::CanBeScored,
};

/// Represents a device instance.
pub struct Device<'a> {
    pub(crate) parent: &'a Instance,
    pub(crate) physical_device: PhysicalDevice,
    pub(crate) graphics_index: u32,
    transfer_index: Option<u32>,
    pub(crate) device: VkDevice,
    graphics: Queue,
    transfer: Option<Queue>,
}

pub fn as_mut_ptr<T>(element: &mut T) -> *mut c_void {
    element as *mut T as *mut c_void
}

impl Instance {
    fn query_physical_device(&self) -> Result<PhysicalDevice, Error> {
        let mut count: u32 = 0;
        match unsafe { vkEnumeratePhysicalDevices(self.instance, &mut count, null_mut()) } {
            VkResult::Success => {}

            VkResult::ErrorOutOfHostMemory => return Err(Error::OutOfHostMemory),
            VkResult::ErrorOutOfDeviceMemory => return Err(Error::OutOfDeviceMemory),

            _ => {
                panic!("vkEnumeratePhysicalDevices failed");
            }
        }

        if count == 0 {
            return Err(Error::NoUsableDevicesPresent);
        }

        let mut devices: Vec<PhysicalDevice> = vec![null_mut(); count as usize];
        match unsafe { vkEnumeratePhysicalDevices(self.instance, &mut count, devices.as_mut_ptr()) } {
            VkResult::Success => {}

            VkResult::ErrorOutOfHostMemory => return Err(Error::OutOfHostMemory),
            VkResult::ErrorOutOfDeviceMemory => return Err(Error::OutOfDeviceMemory),

            _ => {
                panic!("vkEnumeratePhysicalDevices failed");
            }
        }

        let mut scores = Vec::<u16>::new();
        for device in &devices {
            scores.push(device.score());
        }

        if scores.iter().all(|&x| x == 0) {
            return Err(Error::NoUsableDevicesPresent);
        }

        Ok(devices[scores.iter().enumerate().max().map(|(i, _)| i).unwrap()])
    }

    fn create_device(&self, physical_device: PhysicalDevice, graphics: u32, transfer: Option<u32>) -> Result<(VkDevice, Queue, Option<Queue>), Error> {
        let extensions = extension_list![
            "VK_KHR_swapchain\0",
            "VK_KHR_dynamic_rendering\0",
            "VK_KHR_shader_non_semantic_info\0",
            "VK_EXT_extended_dynamic_state\0"
        ];

        let priority = 1.0;

        let queue_create_infos: [DeviceQueueCreateInfo; 2] = [
            DeviceQueueCreateInfo {
                s_type: StructureType::DeviceQueueCreateInfo,
                next: null_mut(),
                flags: 0,
                family_index: graphics,
                count: 1,
                priorities: &priority,
            },
            DeviceQueueCreateInfo {
                s_type: StructureType::DeviceQueueCreateInfo,
                next: null_mut(),
                flags: 0,
                family_index: transfer.unwrap_or(0),
                count: 1,
                priorities: &priority,
            },
        ];

        let mut extended_dynamic_state_feature = PhysicalDeviceExtendedDynamicStateFeatures {
            s_type: StructureType::PhysicalDeviceDynamicRenderingFeatures,
            next: null_mut(),
            extended_dynamic_state: VK_TRUE,
        };

        let mut dynamic_rendering_feature = PhysicalDeviceDynamicRenderingFeatures {
            s_type: StructureType::PhysicalDeviceDynamicRenderingFeatures,
            next: as_mut_ptr(&mut extended_dynamic_state_feature),
            dynamic_rendering: VK_TRUE,
        };

        let create_info = DeviceCreateInfo {
            s_type: StructureType::DeviceCreateInfo,
            next: as_mut_ptr(&mut dynamic_rendering_feature),
            flags: 0,

            queue_create_info_count: if transfer.is_some() { 2 } else { 1 },
            queue_create_infos: queue_create_infos.as_ptr(),

            layer_count: 0,
            layers: null(),

            extension_count: extensions.len() as u32,
            extensions: extensions.as_ptr(),

            features: &DEVICE_FEATURES,
        };

        let mut device: VkDevice = null_mut();
        match unsafe { CreateDevice(physical_device, &create_info, null(), &mut device) } {
            VkResult::Success => {
                let mut graphics_queue: Queue = null_mut();
                unsafe { GetDeviceQueue(device, graphics, 0, &mut graphics_queue) };

                let mut transfer_queue: Queue = null_mut();
                if transfer.is_some() {
                    unsafe { GetDeviceQueue(device, transfer.unwrap(), 0, &mut transfer_queue) };
                }

                Ok((device, graphics_queue, if transfer.is_some() { Some(transfer_queue) } else { None }))
            }

            VkResult::ErrorOutOfHostMemory => Err(Error::OutOfHostMemory),
            VkResult::ErrorOutOfDeviceMemory => Err(Error::OutOfDeviceMemory),
            VkResult::ErrorExtensionNotPresent => Err(Error::ExtensionsMissing),
            VkResult::ErrorLayerNotPresent => Err(Error::LayersMissing),
            VkResult::ErrorDeviceLost => Err(Error::DeviceLost),

            _ => {
                panic!("CreateDevice failed");
            }
        }
    }

    /// Fetches and initializes a device.
    pub fn initialize_device(&self) -> Result<Device, Error> {
        match self.query_physical_device() {
            Ok(physical_device) => {
                let (graphics_index, transfer_index) = physical_device.get_queue_family_indices();
                match self.create_device(physical_device, graphics_index, transfer_index) {
                    Ok((device, graphics, transfer)) => Ok(Device {
                        parent: self,
                        physical_device,
                        graphics_index,
                        transfer_index,
                        device,
                        graphics,
                        transfer,
                    }),

                    Err(err) => Err(err),
                }
            }

            Err(err) => Err(err),
        }
    }
}

impl Drop for Device<'_> {
    fn drop(&mut self) {
        let result = unsafe { WaitForDeviceIdle(self.device) };
        assert_eq!(result, VkResult::Success);

        unsafe {
            DestroyDevice(self.device, null_mut());
        }
    }
}
