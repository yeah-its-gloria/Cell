// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use super::{structures::RenderingInfo, types::CommandBuffer};

pub type CommandBeginRenderingFn = extern "C" fn(buffer: CommandBuffer, info: *const RenderingInfo);
pub type CommandEndRenderingFn = extern "C" fn(buffer: CommandBuffer);
pub type CommandSetCullModeFn = extern "C" fn(buffer: CommandBuffer, mode: u32);
