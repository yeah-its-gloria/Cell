// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use std::{
    sync::{Arc, Mutex},
    thread,
};

use crate::platform::Platform;

pub fn engine_main<F: Fn(Arc<Mutex<Platform>>) + Send + 'static>(entry: F) {
    let platform = Arc::new(Mutex::new(Platform { a: 5 }));

    let platform_clone = platform.clone();
    let thread = thread::Builder::new()
        .name("Title Thread".to_owned())
        .spawn(move || entry(platform_clone))
        .unwrap();

    // TODO: platform work

    while !thread.is_finished() {}
}
