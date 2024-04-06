// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use cell_core::interop::Opaque;

use super::Flags;

mod defaults;
mod features;
mod functions;
mod structures;

pub use features::*;
pub use functions::*;
pub use structures::*;

pub type PhysicalDevice = *mut Opaque;

#[repr(u32)]
pub enum PhysicalDeviceType {
    Other = 0,
    IntegratedGPU = 1,
    DiscreteGPU = 2,
    VirtualGPU = 3,
    CPU = 4,
}

bitflags::bitflags! {
    #[repr(C)]
    #[derive(Clone, Debug)]
    pub struct QueueFlags : Flags {
        const IsGraphics = 0x1;
        const IsCompute = 0x2;
        const IsTransfer = 0x4;
    }
}
