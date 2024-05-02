# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

module_shell_includes = include_directories('Includes')

module_shell_sources = [
    'Sources/Actions.cc',
    'Sources/Controller.cc',
    'Sources/RunDispatch.cc',
    'Sources/Shell.cc',

    'Sources/Controller/DualSense.cc',
    'Sources/Controller/DualShock4.cc',

    'Sources/Controller/SwitchPro/Commands.cc',
    'Sources/Controller/SwitchPro/SwitchPro.cc'
]

module_shell_defines = [
    '-DCELL_MODULES_SHELL_AVAILABLE=1'
]

module_shell_linker_args = []

module_shell_dependencies = [ core ]

if host_machine.system() == 'windows'
    module_shell_sources += [
        'Platform/Windows/Controller/Xbox.cc',

        'Platform/Windows/KeyboardHandler.cc',
        'Platform/Windows/NewDestruct.cc',
        'Platform/Windows/WindowLoop.cc',
        'Platform/Windows/Windows.cc'
    ]

    module_shell_defines += '-Wno-language-extension-token'
    module_shell_linker_args += '-ldwmapi'
elif host_machine.system() == 'darwin'
    module_shell_sources += [
        'Platform/macOS/Events.mm',
        'Platform/macOS/macOS.mm',

        'Platform/macOS/Controller/Xbox.mm'
    ]

    module_shell_linker_args += [
        '-Wl,-framework,Foundation',
        '-Wl,-framework,Cocoa',
        '-Wl,-framework,GameController',
        '-Wl,-framework,QuartzCore'
    ]
elif host_machine.system() == 'linux'
    module_shell_sources += [
        'Platform/Linux/Linux.cc',
        'Platform/Linux/NewDestruct.cc',

        'Platform/Linux/Events/Events.cc',
        'Platform/Linux/Events/Keyboard.cc',
        'Platform/Linux/Events/Pointer.cc',
        'Platform/Linux/Events/Registry.cc',
        'Platform/Linux/Events/Seat.cc',
        'Platform/Linux/Events/XDGToplevel.cc',

        'Platform/Linux/Protocols/cursor-shape-protocol.c',
        'Platform/Linux/Protocols/idle-inhibit-protocol.c',
        'Platform/Linux/Protocols/pointer-constraints-protocol.c',
        'Platform/Linux/Protocols/relative-pointer-protocol.c',
        'Platform/Linux/Protocols/tablet-protocol.c',
        'Platform/Linux/Protocols/xdg-decoration-protocol.c',
        'Platform/Linux/Protocols/xdg-shell-protocol.c',

        'Platform/Linux/Controller/Xbox.cc'
    ]

    module_shell_dependencies += [
        dependency('wayland-client', required: true),
        dependency('xkbcommon', required: true)
    ]
endif

module_shell_library = shared_library('CellShell',
    sources: module_shell_sources,
    include_directories: module_shell_includes,
    link_args: module_shell_linker_args,
    dependencies: module_shell_dependencies
)

module_shell = declare_dependency(
    compile_args: module_shell_defines,
    include_directories: module_shell_includes,
    link_with: module_shell_library,
    dependencies: core
)
