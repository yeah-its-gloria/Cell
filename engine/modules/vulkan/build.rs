// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use std::env;

fn main() {
    let vulkan_sdk_env = "VULKAN_SDK";

    let is_sdk_avail = env::var(vulkan_sdk_env).is_ok();
    let sdk_path = match is_sdk_avail {
        true => env::var(vulkan_sdk_env).unwrap(),
        false => "".to_owned(),
    };

    if cfg!(windows) && is_sdk_avail {
        println!("cargo:rustc-link-search={}\\Lib", sdk_path);
    }

    if cfg!(windows) {
        println!("cargo:rustc-link-lib=vulkan-1");
    } else {
        println!("cargo:rustc-link-lib=vulkan");
    }
}
