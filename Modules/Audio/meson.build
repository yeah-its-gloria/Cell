# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

module_audio_includes = include_directories('Includes')

module_audio_sources = [
    'Sources/CreateSubsystem.cc'
]

module_audio_defines = [
    '-DCELL_MODULES_AUDIO_AVAILABLE=1'
]

module_audio_linker_args = []

module_audio_dependencies = [ core ]

if host_machine.system() == 'windows'
    module_audio_sources += [
        'Sources/WASAPI/Capturer.cc',
        'Sources/WASAPI/CreateLoopback.cc',
        'Sources/WASAPI/CreateRenderer.cc',
        'Sources/WASAPI/DiscoverAvailableRenderers.cc',
        'Sources/WASAPI/FindDevice.cc',
        'Sources/WASAPI/NotificationImpl.cc',
        'Sources/WASAPI/Renderer.cc',
        'Sources/WASAPI/Subsystem.cc'
    ]

    module_audio_defines += '-Wno-language-extension-token'
    module_audio_linker_args += '-lmmdevapi'
elif host_machine.system() == 'darwin'
    module_audio_sources += [
        'Sources/CoreAudio/Capturer.cc',
        'Sources/CoreAudio/Renderer.cc',
        'Sources/CoreAudio/Subsystem.cc'
    ]

    module_audio_linker_args += [
        '-Wl,-framework,Foundation',
        '-Wl,-framework,CoreAudio',
        '-Wl,-framework,AudioToolbox'
    ]
elif host_machine.system() == 'linux'
    module_audio_sources += [
        'Sources/PulseAudio/CreateRenderer.cc',
        'Sources/PulseAudio/Renderer.cc',
        'Sources/PulseAudio/Subsystem.cc'
    ]

    module_audio_dependencies += dependency('pulse', required: true)
endif

module_audio_library = shared_library('CellAudio',
    sources: module_audio_sources,
    include_directories: module_audio_includes,
    link_args: module_audio_linker_args,
    dependencies: module_audio_dependencies
)

module_audio = declare_dependency(
    compile_args: module_audio_defines,
    include_directories: module_audio_includes,
    link_with: module_audio_library,
    dependencies: core
)
