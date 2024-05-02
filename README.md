# Cell
**Cell** is a **video game engine** and an **application development toolkit**, mostly written in C++.

Please note that this project is in an incredibly early state, and that breaking changes will constantly be made.  
There is no guarantee that any application component currently in tree will even launch across all supported platforms.

**Important note: At the moment, support on Linux is __completely broken__. Windows may not function properly. Yeah I develop actively on a Mac**

## Building
Cell supports Windows and Linux, in particular Arch Linux and Ubuntu. 
Note that some platform implementations might be less functional than others (I try my best to keep them all on the same page, at least API wise), i.e., the engine might abort for certain functionality.

Cell has the following dependencies:
| Name | Minimum Version | Platform | Notes | Ubuntu Package | Arch Linux Package |
|-|-|-|-|-|-|
| [LLVM](https://llvm.org) | 16.0.0 | All | Specifically clang and lld are needed. clang-cl may be usable on Windows, but untested. Other compilers aren't tested or supported. | `clang`, `lld`, `llvm` | `clang`, `lld`, `llvm` |
| [Meson](https://mesonbuild.org/) | 1.4.0 | All | | `meson` | `meson` |
| [Ninja](https://ninja-build.org/) | 1.11.1 | All | Recommended backend for Meson. Xcode works too, if you can stand it. Visual Studio is broken. | `ninja-build` | `ninja` ||
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

## Core
The core implements a platform-agnostic interface for the editor, all modules, titles and utilities.

## Editor
The editor serves as a creation utility for various workloads.

## Modules
| Name | Implementation | Notes |
|-|-|-|
| Audio | WASAPI (Windows), CoreAudio (macOS), PulseAudio (Linux) | |
| DataManagement | self-contained | |
| Mathematics | self-contained | |
| OpenXR | OpenXR 1.0 | Supports D3D12 and Vulkan |
| Renderer | D3D12 (Windows), Metal (macOS), Vulkan 1.2 (all) | Implemented Vulkan WSIs: Win32, Metal, Wayland |
| Shell | Windows, AppKit (macOS), Wayland (Linux), HID, etc | Serves input implementations |

A content packing utility is also available.

## In-tree Projects
The projects in Cell's source tree are meant to be a broad testing ground.

## Testing
The engine, modules and projects may use tests.

## Contributing & Licensing
The code style should be straightforward, keep in line with the other code.

The engine itself, in-tree engine modules and my projects code follow the BSD 2-Clause license.

Modules are your work, and do not need to be in the engine directory. Titles can have their own modules.  
Title and out of tree module licensing is up to you, as long as it doesn't interfere with the BSD 2-Clause license.
