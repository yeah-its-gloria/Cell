// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use super::{structures::RenderingInfo, types::CommandBuffer};

pub type CommandBeginRenderingFn =
    unsafe fn(command_buffer: CommandBuffer, rendering_info: *const RenderingInfo);

pub type CommandEndRenderingFn = unsafe fn(command_buffer: CommandBuffer);
