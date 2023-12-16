// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use std::env::var;

fn main() {
    if cfg!(windows) {
        println!("cargo:rerun-if-env-changed=VULKAN_SDK");

        match var("VULKAN_SDK") {
            Ok(path) => println!("cargo:rustc-link-search={}\\Lib", path),
            _ => {}
        }

        println!("cargo:rustc-link-lib=vulkan-1");
    } else {
        println!("cargo:rustc-link-lib=vulkan");
    }
}
