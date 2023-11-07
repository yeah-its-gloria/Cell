// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

mod device;
mod ffi;
mod instance;

pub struct Instance {
    instance: crate::ffi::types::Instance,
    begin_render: crate::ffi::dynamic::CommandBeginRenderingFn,
    end_render: crate::ffi::dynamic::CommandEndRenderingFn,
}

#[derive(Debug)]
pub enum Error {
    OutOfHostMemory,
    OutOfDeviceMemory,

    Unknown,
}
