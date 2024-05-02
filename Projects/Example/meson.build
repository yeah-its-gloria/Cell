# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project_example_soures = [
    'Sources/Example.cc',

    'Sources/AudioThread.cc',
    'Sources/InputController.cc',

    'Sources/Renderer/Thread.cc',
    'Sources/Renderer/Tools/GenerateRenderCommands.cc',
    'Sources/Renderer/Tools/LoadShader.cc',
    'Sources/Renderer/Tools/LoadTexture.cc',
    'Sources/Renderer/Tools/SetUpResources.cc'
]

executable('CellExample',
    sources: project_example_soures,
    dependencies: [ core, core_bootstrapper, module_audio, module_mathematics, module_shell, module_renderer ],

    install: true,

    win_subsystem: 'windows'
)

meson.add_install_script('install.sh')
