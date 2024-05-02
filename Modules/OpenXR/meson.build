# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

openxr_dep = dependency('openxr', required: false)
if not openxr_dep.found()
    subdir_done()
endif

module_openxr_includes = include_directories('Includes')

module_openxr_sources = [
    'Sources/Instance.cc',
    'Sources/PrepSession.cc',
    'Sources/Session.cc'
]

module_openxr_defines = [
    '-DCELL_MODULES_OPENXR_AVAILABLE=1',

    '-DXR_USE_GRAPHICS_API_VULKAN=1'
]

module_openxr_linker_args = []

if host_machine.system() == 'windows'
    module_openxr_defines += '-DXR_USE_GRAPHICS_API_D3D12=1'
endif

module_openxr_library = shared_library('CellOpenXR',
    sources: module_openxr_sources,
    include_directories: module_openxr_includes,
    link_args: module_openxr_linker_args,
    dependencies: [ core, module_mathematics, module_renderer, openxr_dep ]
)

module_openxr = declare_dependency(
    compile_args: module_openxr_defines,
    include_directories: module_openxr_includes,
    link_with: module_openxr_library,
    dependencies: [ core, module_mathematics, module_renderer, openxr_dep ]
)
