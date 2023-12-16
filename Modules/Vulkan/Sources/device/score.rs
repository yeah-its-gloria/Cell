// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use super::{
    super::ffi::{
        get_major_version, get_minor_version, vkGetPhysicalDeviceFeatures2, vkGetPhysicalDeviceProperties, PhysicalDevice, PhysicalDeviceDynamicRenderingFeatures,
        PhysicalDeviceExtendedDynamicStateFeatures, PhysicalDeviceFeaturesEx, PhysicalDeviceProperties, PhysicalDeviceType, VK_FALSE, VK_TRUE,
    },
    as_mut_ptr, HasFamilyQueueIndices, DEVICE_FEATURES,
};

pub trait CanBeScored {
    fn score(&self) -> u16;
}

impl CanBeScored for PhysicalDevice {
    fn score(&self) -> u16 {
        let mut properties = PhysicalDeviceProperties::default();
        unsafe {
            vkGetPhysicalDeviceProperties(*self, &mut properties);
        }

        if get_major_version(properties.api_version) < 1 || get_minor_version(properties.api_version) < 3 {
            return 0;
        }

        let mut score = match properties.device_type {
            PhysicalDeviceType::DiscreteGPU => 400,
            PhysicalDeviceType::VirtualGPU | PhysicalDeviceType::IntegratedGPU => 200,
            PhysicalDeviceType::CPU => 100,
            _ => 0,
        };

        if properties.limits.max_push_constants_size >= 256 {
            score += 200;
        }

        let mut extended_dynamic_state_feature = PhysicalDeviceExtendedDynamicStateFeatures::default();

        let mut dynamic_rendering_feature = PhysicalDeviceDynamicRenderingFeatures::default();
        dynamic_rendering_feature.next = as_mut_ptr(&mut extended_dynamic_state_feature);

        let mut features_ex = PhysicalDeviceFeaturesEx::default();
        features_ex.next = as_mut_ptr(&mut dynamic_rendering_feature);

        unsafe {
            vkGetPhysicalDeviceFeatures2(*self, &mut features_ex);
        }

        // may God forgive me for the solutions I found while sinning
        // this might just be worse than the stapling last week

        let missing_features = features_ex
            .features
            .into_iter()
            .zip(DEVICE_FEATURES)
            .filter(|&(device, requested)| if requested == VK_TRUE && device == VK_FALSE { true } else { false })
            .count();

        if missing_features > 0 || dynamic_rendering_feature.dynamic_rendering == VK_FALSE || extended_dynamic_state_feature.extended_dynamic_state == VK_FALSE {
            return 0;
        }

        let (_, transfer) = self.get_queue_family_indices();
        if transfer.is_some() {
            score += 200;
        }

        score
    }
}
