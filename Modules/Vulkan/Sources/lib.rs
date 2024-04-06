// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

//! Vulkan module for Cell; implements Vulkan support, primarily rendering

#![no_std]
extern crate alloc;

mod device;
mod image;
mod instance;
mod render_target;
mod wsi_target;

mod ffi;

pub use device::*;
pub use image::*;
pub use render_target::*;
pub use wsi_target::*;

pub use instance::Instance;

pub mod surface_details {
    pub use super::{
        ffi::{AllocationCallbacks, StructureType, SurfaceKHR, VkInstance, VkResult},
        SurfaceProvider,
    };
}

/// Error codes used across Vulkan code
#[derive(Debug)]
pub enum Error {
    OutOfHostMemory,
    OutOfDeviceMemory,
    ExtensionsMissing,
    LayersMissing,
    IncompatibleDriver,
    NoUsableDevicesPresent,
    DeviceLost,
    SurfaceLost,
    ShellSessionNotUsable,
}
