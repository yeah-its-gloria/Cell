// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use std::{
    env::{split_paths, var, var_os},
    fs::{canonicalize, read_to_string, write},
    path::{Path, PathBuf},
    process::Command,
};

fn discover_program<P>(exe_name: P) -> Option<PathBuf>
where
    P: AsRef<Path>,
{
    var_os("PATH").and_then(|paths| {
        split_paths(&paths).find_map(|dir| {
            let full_path = dir.join(&exe_name);
            if full_path.is_file() {
                Some(full_path)
            } else {
                None
            }
        })
    })
}

pub fn generate_manifests(workspace_offset: &str) {
    if cfg!(windows) {
        let proj_dir = canonicalize(format!("{}\\{workspace_offset}", var("CARGO_MANIFEST_DIR").unwrap())).unwrap();
        let out_dir = var("OUT_DIR").unwrap();

        let base_dir = format!("{}\\BuildTools\\Content\\Windows", proj_dir.to_str().unwrap());

        let resource_src = format!("{base_dir}\\Manifest.rc");
        let manifest = format!("{base_dir}\\Manifest.xml");

        let resource_gen = format!("{out_dir}\\Manifest.rc");
        let resource_out = format!("{out_dir}\\Manifest.res");

        let mut resource_content = read_to_string(&resource_src).unwrap();

        for (ident, value) in [
            ("@NAME@", var("CARGO_PKG_NAME").unwrap()),
            ("@DESC@", var("CARGO_PKG_DESCRIPTION").unwrap()),
            ("@AUTHORS@", var("CARGO_PKG_AUTHORS").unwrap()),
            ("@VMAJOR@", var("CARGO_PKG_VERSION_MAJOR").unwrap()),
            ("@VMINOR@", var("CARGO_PKG_VERSION_MINOR").unwrap()),
            ("@VPATCH@", var("CARGO_PKG_VERSION_PATCH").unwrap()),
        ] {
            resource_content = resource_content.replace(ident, &value);
        }

        write(&resource_gen, resource_content).unwrap();

        let mut result = false;
        for compiler in ["llvm-rc.exe", "rc.exe"] {
            result = match discover_program(compiler) {
                Some(path) => Command::new(&path)
                    .current_dir(&base_dir)
                    .arg(&resource_gen)
                    .arg("-fo")
                    .arg(&resource_out)
                    .status()
                    .unwrap()
                    .success(),
                _ => false,
            };

            if result {
                break;
            }
        }

        assert!(result);

        println!("cargo:rustc-link-arg={resource_out}");
        println!("cargo:rustc-link-arg=/MANIFEST:EMBED");
        println!("cargo:rustc-link-arg=/MANIFESTINPUT:{manifest}");

        println!("cargo:rerun-if-changed={resource_src}");
        println!("cargo:rerun-if-changed={manifest}");
    }
}
