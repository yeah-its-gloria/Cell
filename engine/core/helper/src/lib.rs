// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use proc_macro::TokenStream;

#[proc_macro_attribute]
pub fn title_entry(_attr: TokenStream, item: TokenStream) -> TokenStream {
    let mut prelude: TokenStream =
        format!("fn main() {{ cell_core::entry::engine_main(&entry); }}")
            .parse()
            .unwrap();

    prelude.extend(item);
    return prelude;
}
