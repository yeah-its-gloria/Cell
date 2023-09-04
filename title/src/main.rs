// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use std::sync::{Arc, Mutex};

use cell_core::platform::Platform;
use cell_core_helper::title_entry;
use cell_modules_vulkan::Instance;

#[title_entry]
fn entry(_platform: Arc<Mutex<Platform>>) {
    let mut instance = Instance::new().unwrap();

    instance.initialize_device().unwrap();

    instance.destroy();
}
