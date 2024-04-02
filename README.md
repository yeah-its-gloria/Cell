# Cell
**Cell** is a **video game engine** and an **application development toolkit**, mostly written in C++.

Please note that this project is in an incredibly early state, and that breaking changes will constantly be made.  
There is no guarantee that any application component currently in tree will even launch across all supported platforms.

**Important note: At the moment, support on Linux is __completely broken__.**

## Building
Cell supports Windows and Linux, in particular Arch Linux and Ubuntu. 
Note that some platform implementations might be less functional than others (I try my best to keep them all on the same page, at least API wise), i.e., the engine might abort for certain functionality.

Cell has the following dependencies:
| Name | Minimum Version | Platform | Notes | Ubuntu Package | Arch Linux Package |
|-|-|-|-|-|-|
| [LLVM](https://llvm.org) | 16.0.0 | All | Specifically clang and lld are needed. clang-cl can be used on Windows, if desired. Other compilers are **not officially** supported. | `clang`, `lld`, `llvm` | `clang`, `lld`, `llvm` |
| [CMake](https://cmake.org/) | 3.24.0 | All | Requires a generator of some kind. | `cmake` | `cmake` |
| [Ninja](https://ninja-build.org/) | 1.11.1 | All | Recommended generator for CMake. | `ninja-build` | `ninja` |
| [Rust](https://rust-lang.org) via [cargo](https://github.com/rust-lang/cargo) | 1.75.0 | All | [rustup](https://rustup.rs/) is recommended for managing Rust installations. | `cargo`, `rustc` | `rust` |
| | | | | | |
| [Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/) | 10.0.22621.0 | Windows | Should be installed alongside MSVC v143. | | |
| | | | | | |
| [Wayland](https://wayland.freedesktop.org/) | 1.3 | Linux | Official support for compositors is currently limited to [KDE (KWin)](https://kde.org/). | `libwayland-dev` | `wayland` | 
| [xkbcommon](https://xkbcommon.org/) | 1.6.0 | Linux | | `libxkbcommon-dev` | `libxkbcommon` |
| [PulseAudio](https://www.freedesktop.org/wiki/Software/PulseAudio/) | 16.1 | Linux | Official support for server implementations is limited to [pipewire](https://pipewire.org/). | `libpulse-dev` | `libpulse` |
| | | | | | |
| [Vulkan](https://www.vulkan.org/) [headers](https://github.com/KhronosGroup/Vulkan-Headers) and [loader](https://github.com/KhronosGroup/Vulkan-Loader) | 1.3.268 | All | LunarG offers an [SDK package](https://vulkan.lunarg.com/sdk/home#windows) for Windows. | `libvulkan-dev` | `vulkan-headers`, `vulkan-loader` |
| `spirv-link` from [SPIR-V Tools](https://github.com/KhronosGroup/SPIRV-Tools) | 2023.3 | All | | `spirv-tools` | `spirv-tools` |
| `glslc` from [shaderc](https://github.com/google/shaderc/) | 2023.2 | All | | `glslc` | `shaderc` |
| | | | | | |
| [OpenXR](https://www.khronos.org/openxr/) [SDK](https://github.com/KhronosGroup/OpenXR-SDK) | 1.0.33 | All | Not required; if OpenXR isn't found, Cell will build without VR support. | `libopenxr-dev` | `openxr` |

Requires either Windows 10 or newer, Ubuntu 24.04 or newer, or an up-to-date Arch Linux installation.  
Other Linux distributions might not be as well supported, and other Unix-ish-likes are likely broken and do not receive full support.

You can additionally configure the engine further through CMake. I recommend using a multi-configuration generator, like `Ninja Multi-Config`.
 
| Setting | Default Value | Intent |
|-|-|-|
| `CELL_ENABLE_EDITOR` | ON | Enables the editor being built. |
| `CELL_ENABLE_PROJECTS` | ON | Enables the in tree project being built. |
| `CELL_ENABLE_TESTS` | ON | Enables tests across the entire engine. |
| `CELL_ENABLE_TESTS_LIKELY_FAILURE` | OFF | Enables tests that are likely to fail or not properly implemented yet. |
| `CELL_ENABLE_UTILITIES` | ON | Enables utilities being built. |
| `CELL_ENABLE_STATIC_BUILD` | ON | Builds all library components statically. Particularly recommended for release builds. |

## Core
The core implements a platform-agnostic interface for the editor, all modules, titles and utilities.

| Setting | Default Value | Intent |
|-|-|-|
| `CELL_CORE_SKIP_ASSERT` | OFF | Disables checking assertions. Takes priority over all assertion options. |
| `CELL_CORE_USE_EXTERNAL_ASSERT` | OFF | Calls `assert()` in case an assertion fails. Takes priority over `CELL_USE_PANIC_ASSERT` |
| `CELL_CORE_USE_PANIC_ASSERT` | ON | Calls `Cell::System::Panic()` in case an assertion fails. |

## Editor
The editor serves as a creation utility for various workloads.

## Modules
| Name | Implementation | Notes |
|-|-|-|
| Audio | WASAPI (Windows), PulseAudio (Linux) | |
| D3D12 | DirectX 12.1 | Windows only |
| DataManagement | self-contained | |
| Mathematics | self-contained | |
| OpenXR | OpenXR 1.0 | Supports D3D12 and Vulkan |
| Shell | Windows, Wayland (Linux), HID, etc | Serves input implementations |
| Vulkan | Vulkan 1.2 | Implemented WSIs: Win32 and Wayland |

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
