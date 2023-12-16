// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use cell_module_shell::{create_default_shell, KeyboardButton, Shell};
use cell_module_vulkan::Instance;

fn main() {
    let mut shell = create_default_shell("Cell - Hi Aurelia", 1280, 720).unwrap();

    let instance = Instance::new().unwrap();
    let device = instance.initialize_device().unwrap();

    let mut target = device.create_wsi_target(&shell).unwrap();
    target.set_up_rendering(&shell).unwrap();

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
}
