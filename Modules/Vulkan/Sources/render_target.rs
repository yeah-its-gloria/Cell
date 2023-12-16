// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use crate::{
    ffi::{Fence, Image, Semaphore},
    Error,
};

#[derive(Clone, Copy)]
pub(crate) struct SynchronizationComponents {
    pub image_available: Semaphore,
    pub render_finished: Semaphore,
    pub in_flight: Fence,
}

/// Represents an acquired image, usable for rendering.
pub struct AcquiredImage {
    image: Image,
    sync: Option<SynchronizationComponents>,
}

/// Interfaces for targets for rendering procedures.
pub trait RenderTarget {
    /// Acquires the next image.
    fn acquire_next() -> Result<AcquiredImage, Error>;

    /// Issues a present.
    fn present() -> Result<(), Error>;
}
