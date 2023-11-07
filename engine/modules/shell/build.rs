// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

fn main() {
    if cfg!(windows) {
        const LIBRARIES: [&str; 3] = ["dwmapi", "gdi32", "user32"];

        for library in LIBRARIES {
            println!("cargo:rustc-link-lib={}", library);
        }
    }
}
