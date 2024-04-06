// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#[repr(u32)]
pub enum AttachmentLoadOp {
    Load = 0,
    Clear = 1,
    DoNotCare = 2,
}

#[repr(u32)]
pub enum AttachmentStoreOp {
    Store = 0,
    DoNotCare = 1,
    None = 1000301000,
}

#[repr(u32)]
pub enum ResolveModeFlag {
    None = 0,
    SampleZero = 1,
    Average = 2,
    Min = 4,
    Max = 8,
}

#[repr(u32)]
pub enum RenderingFlag {
    ContentsSecondaryCommandBuffers = 0x00000001,
    Suspending = 0x00000002,
    Resuming = 0x00000004
}
