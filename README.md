# Cell
**Cell** is a **video game engine** and an **application development toolkit**, written in Rust.

Please note that this project is in an incredibly early state, and that breaking changes will constantly be made.  
There is no guarantee that any application component currently in tree will even launch across all supported platforms.

## Building
At this time, Cell only functions on Windows.  
Linux support is planned to return.

## Components
Cell is built with a wide range of components.

### Core
The core package implements all base functionality needed across all modules, utilities and projects.
 
### Editor
The editor utility is meant to provide an editor UI for managing content, scenes, UI and more.  
Currently, the editor is non-functional.

### Modules
Modules serve as coherent blocks of functionality.

| Name           | Implementation      | Notes                        |
|----------------|---------------------|------------------------------|
| Shell          | Windows, HID, etc   | Serves input implementations |
| Vulkan         | Vulkan 1.2          | Implemented WSIs: Win32      |

### In-tree project
The example project in the tree serves as a testing playground for the most part.

## Contributing & Licensing
The code style should be straightforward, keep in line with the other code.  

The engine itself, in-tree engine modules and my project code follow the BSD 2-Clause license.

Modules are your work, and do not need to be in the engine directory. projects can have their own modules.  
Project and out of tree module licensing is up to you, as long as it doesn't interfere with the BSD 2-Clause license.
