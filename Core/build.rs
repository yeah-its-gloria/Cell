// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

fn main() {
    if cfg!(windows) {
        // Fixes unresolved externals
        println!("cargo:rustc-link-lib={}", if cfg!(debug_assertions) { "libcmtd" } else { "libcmt" });
    }
}
