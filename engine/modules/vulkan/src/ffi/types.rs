// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use cell_core::interop::Opaque;

pub type Bool32 = u32;
pub type DeviceSize = u64;

pub const VK_FALSE: Bool32 = 0;
pub const VK_TRUE: Bool32 = 1;

pub type CommandBuffer = *mut Opaque;
pub type ImageView = *mut Opaque;
pub type Instance = *mut Opaque;
pub type PhysicalDevice = *mut Opaque;
