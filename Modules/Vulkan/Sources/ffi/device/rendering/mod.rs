// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use core::ffi::c_void;

use crate::ffi::{CommandBuffer, ImageLayout, Rect2D, StructureType, VkImageView};

mod enumerations;
pub use enumerations::*;

#[repr(C)]
#[derive(Clone, Copy)]
pub union ClearColorValue {
    pub float32: [f32; 4],
    pub int32: [i32; 4],
    pub uint32: [u32; 4],
}

#[repr(C)]
#[derive(Clone, Copy)]
pub struct ClearDepthStencilValue {
    pub depth: f32,
    pub stencil: u32,
}

#[repr(C)]
pub union ClearValue {
    pub color: ClearColorValue,
    pub depth_stencil: ClearDepthStencilValue,
}

#[repr(C)]
pub struct RenderingAttachmentInfo {
    pub s_type: StructureType,
    pub next: *mut c_void,
    pub image_view: VkImageView,
    pub image_layout: ImageLayout,
    pub resolve_mode: ResolveModeFlag,
    pub resolve_image_view: VkImageView,
    pub resolve_image_layout: ImageLayout,
    pub load_op: AttachmentLoadOp,
    pub store_op: AttachmentStoreOp,
    pub clear_value: ClearValue,
}

#[repr(C)]
pub struct RenderingInfo {
    pub s_type: StructureType,
    pub next: *mut c_void,
    pub flags: RenderingFlag,
    pub render_area: Rect2D,
    pub layer_count: u32,
    pub view_mask: u32,
    pub attachment_count: u32,
    pub color_attachments: *const RenderingAttachmentInfo,
    pub depth_attachment: *const RenderingAttachmentInfo,
    pub stencil_attachment: *const RenderingAttachmentInfo,
}

pub type CommandBeginRenderingFn = extern "C" fn(buffer: CommandBuffer, info: *const RenderingInfo);
pub type CommandEndRenderingFn = extern "C" fn(buffer: CommandBuffer);
pub type CommandSetCullModeFn = extern "C" fn(buffer: CommandBuffer, mode: u32);
