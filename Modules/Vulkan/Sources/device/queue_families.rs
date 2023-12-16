// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use alloc::{vec, vec::Vec};
use core::ptr::null_mut;

use crate::ffi::{vkGetPhysicalDeviceQueueFamilyProperties, PhysicalDevice, QueueFamilyProperties, QueueFlags};

pub trait HasFamilyQueueIndices {
    fn get_queue_family_indices(&self) -> (u32, Option<u32>);
}

impl HasFamilyQueueIndices for PhysicalDevice {
    fn get_queue_family_indices(&self) -> (u32, Option<u32>) {
        let mut count = 0;
        unsafe {
            vkGetPhysicalDeviceQueueFamilyProperties(*self, &mut count, null_mut());
        }

        let mut properties: Vec<QueueFamilyProperties> = vec![QueueFamilyProperties::default(); count as usize];
        unsafe {
            vkGetPhysicalDeviceQueueFamilyProperties(*self, &mut count, properties.as_mut_ptr());
        }

        let mut graphics_queue: Option<u32> = None;
        let mut transfer_queue: Option<u32> = None;
        for (position, property) in properties.iter().enumerate() {
            if graphics_queue.is_none() && property.flags.contains(QueueFlags::IsGraphics) {
                graphics_queue = Some(position as u32);
            }

            if transfer_queue.is_none() && property.flags.contains(QueueFlags::IsTransfer) && graphics_queue.is_some_and(|x| x != position as u32) {
                transfer_queue = Some(position as u32);
            }

            if graphics_queue.is_some() && transfer_queue.is_some() {
                break;
            }
        }

        assert!(graphics_queue.is_some());
        return (graphics_queue.unwrap(), transfer_queue);
    }
}
