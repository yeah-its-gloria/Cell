# Cell
**Cell** is a **video game engine** and an **application development toolkit**, mostly written in C++.

Please note that this project is in an incredibly early state, and that breaking changes will constantly be made.  
There is no guarantee that any application component currently in tree will even launch across all supported platforms.

## Building
Cell supports Windows and Linux.  
Note that some platform implementations might be less functional than others (I try my best to keep them all on the same page, at least API wise), i.e., the engine might abort for certain functionality (as an example, folder walking is not yet implemented on Linux, but somewhat implemented on Windows).

Cell requires the following components to be built:

| Name                        | Version    | Notes                                                                                                                                                                                                          |
|-----------------------------|------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| [LLVM](https://llvm.org)    | \>= 16.0.0 | Specifically clang and lld are needed. clang-cl can be used on Windows, if desired. Other compilers are **not officially** supported. clang-format and clang-tidy are recommended tools to keep code in order. |
| [CMake](https://cmake.org/) | \>= 3.20.0 | Requires an underlying generator; I recommend [Ninja](https://ninja-build.org/).                                                                                                                               |

Cell also requires the Windows SDK on Windows.  
On Linux, Cell requires Wayland and `libxkbcommon`. There is no support for x11, and I do not intend on implementing any.

You can additionally configure the engine further through CMake. I recommend using a multi-configuration generator, like `Ninja Multi-Config`.
 
| Property                   | Default Value | Intent                                                                    |
|----------------------------|---------------|---------------------------------------------------------------------------|
| `CELL_ENABLE_EDITOR`       | ON            | Enables the editor being built.                                           |
| `CELL_ENABLE_PROJECT`      | ON            | Enables the in tree project being built.                                  |
| `CELL_ENABLE_TESTS`        | ON            | Enables tests across the entire engine.                                   |
| `CELL_ENABLE_UTILITIES`    | ON            | Enables utilities being built.                                            |
| `CELL_ENABLE_STATIC_BUILD` | OFF           | Builds all library components statically. Recommended for release builds. |

## Core
The core implements a platform-agnostic interface for the editor, all modules, titles and utilities.

## Editor
The editor serves as a creation utility for various workloads.

## Modules
| Name           | Implementation                       | Notes                                         |
|----------------|--------------------------------------|-----------------------------------------------|
| Audio          | WASAPI (Windows), PulseAudio (Linux) |                                               |
| D3D12          | DirectX 12.1                         | Windows only                                  |
| DataManagement | self-contained                       |                                               |
| JavaScript     | QuickJS                              | Includes some custom patches                  |
| Mathematics    | self-contained                       | Has hardware acceleration on x86_64 platforms |
| OpenXR         | OpenXR 1.0                           | Vulkan implemented                            |
| Shell          | Windows, Wayland (Linux), HID, etc   | Serves input implementations                  |
| Vulkan         | Vulkan 1.2                           | Implemented WSIs: Win32 and Wayland           |

A content packing utility is also available.

## In-tree Project
The project in Cell's source tree is meant to be a broad testing ground.

## Testing
The engine, modules and projects may use tests.

## Contributing & Licensing
The code style should be straightforward, keep in line with the other code.

The engine itself, in-tree engine modules and my projects code follow the BSD 2-Clause license.

Modules are your work, and do not need to be in the engine directory. Titles can have their own modules.  
Title and out of tree module licensing is up to you, as long as it doesn't interfere with the BSD 2-Clause license.
