// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#[repr(i32)]
#[derive(Debug, PartialEq)]
pub enum VkResult {
    Success = 0,
    NotReady = 1,
    Timeout = 2,
    EventSet = 3,
    EventReset = 4,
    Incomplete = 5,
    ErrorOutOfHostMemory = -1,
    ErrorOutOfDeviceMemory = -2,
    ErrorDeviceLost = -4,
    ErrorLayerNotPresent = -6,
    ErrorExtensionNotPresent = -7,
    ErrorIncompatibleDriver = -9,
    ErrorSurfaceLost = -1000000000,
}
