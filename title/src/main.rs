// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use cell_modules_shell::{create_shell, Shell, Error};
use cell_modules_vulkan::Instance;

fn main() {
    let shell = create_shell("Cell - Hi Aurelia").unwrap();

    let mut instance = Instance::new().unwrap();

    instance.initialize_device().unwrap();

    loop {
        let result = shell.run_dispatch();
        if result.is_err_and(|e| e == Error::RequestedQuit) {
            break
        }
    }
}
