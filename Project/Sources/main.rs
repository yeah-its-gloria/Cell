// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#![no_std]
#![no_main]

use cell_module_shell::{create_default_shell, KeyboardButton, Shell};
use cell_module_vulkan::Instance;

#[no_mangle]
pub extern "C" fn main() -> i32 {
    let mut shell = create_default_shell("Cell - Hi Aurelia", 1280, 720).unwrap();

    let instance = Instance::new().unwrap();
    let device = instance.initialize_device().unwrap();

    let mut target = device.create_wsi_target(&shell).unwrap();
    target.set_up_rendering(&shell).unwrap();

    let _texture = device.create_image(1024, 1024).unwrap();

    loop {
        let still_active = shell.run_dispatch().unwrap();
        if !still_active {
            break;
        }

        let keys = shell.retrieve_keys();
        if keys == KeyboardButton::Escape {
            break;
        }
    }

    0
}
