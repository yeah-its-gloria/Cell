// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use crate::ffi::Flags;

use super::DeviceSize;

#[repr(C)]
pub struct MemoryRequirements {
    pub size: DeviceSize,
    pub alignment: DeviceSize,
    pub type_bits: u32,
}

#[repr(C)]
#[derive(Clone, Copy)]
pub struct MemoryHeap {
    pub size: DeviceSize,
    pub flags: MemoryHeapFlags,
}

#[repr(C)]
#[derive(Clone, Copy)]
pub struct MemoryType {
    pub property_flags: u32,
    pub heap_index: u32,
}

bitflags::bitflags! {
    #[repr(C)]
    #[derive(Clone, Copy, Debug)]
    pub struct MemoryHeapFlags : Flags {
        const None = 0;

        const IsDeviceLocal = 1 << 0;
    }

    #[repr(C)]
    #[derive(Clone, Copy, Debug)]
    pub struct MemoryPropertyFlags : Flags {
        const IsDeviceLocal = 1 << 0;
        const IsHostVisible = 1 << 1;
        const IsHostCoherent = 1 << 2;
        const IsHostCached = 1 << 3;
        const IsLazilyAllocated = 1 << 4;
        const IsProtected = 1 << 5;
    }
}

impl Default for MemoryRequirements {
    fn default() -> Self {
        MemoryRequirements {
            size: 0,
            alignment: 0,
            type_bits: 0,
        }
    }
}
