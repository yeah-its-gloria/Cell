// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use std::env::var;

fn main() {
    if cfg!(windows) {
        println!("cargo:rerun-if-env-changed=VULKAN_SDK");

        match var("VULKAN_SDK") {
            Ok(path) => println!("cargo:rustc-link-search={}\\Lib", path),
            _ => { }
        }

        for library in ["dwmapi", "gdi32", "user32", "vulkan-1"] {
            println!("cargo:rustc-link-lib={}", library);
        }
    } else {
        for library in ["wayland-client", "vulkan"] {
            println!("cargo:rustc-link-lib={}", library);
        }
    }
}
