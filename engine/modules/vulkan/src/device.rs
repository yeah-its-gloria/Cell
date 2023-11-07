use std::ptr::null_mut;

use crate::{
    ffi::{
        enumerations::PhysicalDeviceType,
        functions::{
            EnumeratePhysicalDevices, GetPhysicalDeviceFeatures, GetPhysicalDeviceProperties,
        },
        result::VkResult,
        structures::{
            PhysicalDeviceDynamicRenderingFeatures, PhysicalDeviceFeatures,
            PhysicalDeviceProperties,
        },
        types::{PhysicalDevice, VK_FALSE},
        versioning,
    },
    Error, Instance,
};

fn score_physical_device(_device: PhysicalDevice) -> u16 {
    let mut properties = PhysicalDeviceProperties::default();
    unsafe {
        GetPhysicalDeviceProperties(_device, &mut properties);
    }

    if versioning::get_major_version(properties.api_version) < 1
        || versioning::get_minor_version(properties.api_version) < 3
    {
        return 0;
    }

    let mut score = match properties.device_type {
        PhysicalDeviceType::DiscreteGPU => 400,
        PhysicalDeviceType::IntegratedGPU => 200,
        PhysicalDeviceType::VirtualGPU => 200,
        PhysicalDeviceType::CPU => 200,
        _ => 0,
    };

    if properties.limits.max_push_constants_size >= 256 {
        score += 200;
    }

    let mut dynamic_rendering_feature = PhysicalDeviceDynamicRenderingFeatures::default();
    let mut features = PhysicalDeviceFeatures::default2(&mut dynamic_rendering_feature);
    unsafe {
        GetPhysicalDeviceFeatures(_device, &mut features);
    }

    if features.full_draw_index_uint32 == VK_FALSE
        || features.independent_blend == VK_FALSE
        || features.geometry_shader == VK_FALSE
        || features.tessellation_shader == VK_FALSE
        || features.logic_op == VK_FALSE
        || features.depth_clamp == VK_FALSE
        || features.fill_mode_non_solid == VK_FALSE
        || features.wide_lines == VK_FALSE
        || features.multi_viewport == VK_FALSE
        || features.shader_float64 == VK_FALSE
        || features.shader_int64 == VK_FALSE
        || features.shader_int16 == VK_FALSE
        || dynamic_rendering_feature.dynamic_rendering == VK_FALSE
    {
        return 0;
    }

    // TODO: queue families

    println!("stuff");
    return score;
}

impl Instance {
    fn query_physical_device(self: &mut Self) -> Result<(), Error> {
        let mut count: u32 = 0;
        match unsafe { EnumeratePhysicalDevices(self.instance, &mut count, null_mut()) } {
            VkResult::Success => {}

            VkResult::ErrorOutOfHostMemory => {
                return Err(Error::OutOfHostMemory);
            }
            VkResult::ErrorOutOfDeviceMemory => {
                return Err(Error::OutOfDeviceMemory);
            }
            _ => return Err(Error::Unknown),
        }

        let mut devices: Vec<PhysicalDevice> = vec![null_mut(); count as usize];
        match unsafe { EnumeratePhysicalDevices(self.instance, &mut count, devices.as_mut_ptr()) } {
            VkResult::Success => {}

            VkResult::ErrorOutOfHostMemory => {
                return Err(Error::OutOfHostMemory);
            }
            VkResult::ErrorOutOfDeviceMemory => {
                return Err(Error::OutOfDeviceMemory);
            }
            _ => return Err(Error::Unknown),
        }

        for device in devices {
            score_physical_device(device);
        }

        Ok(())
    }

    pub fn initialize_device(self: &mut Self) -> Result<(), Error> {
        return self.query_physical_device();
    }
}
