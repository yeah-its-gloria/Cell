# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

module_mathematics_includes = include_directories('Includes')

module_mathematics_sources = [
    'Sources/Checks.cc',

    'Sources/Matrix4x4/Shared.hh',

    'Sources/Matrix4x4/AffineTransforms.cc',
    'Sources/Matrix4x4/Constructors.cc',
    'Sources/Matrix4x4/Functions.cc',
    'Sources/Matrix4x4/Operations.cc',
    'Sources/Matrix4x4/Utilities.cc',

    'Sources/Quaternion/Constructors.cc',
    'Sources/Quaternion/Functions.cc',
    'Sources/Quaternion/Operations.cc',

    'Sources/Vector2/Constructors.cc',
    'Sources/Vector2/Functions.cc',
    'Sources/Vector2/Operations.cc',

    'Sources/Vector3/Constructors.cc',
    'Sources/Vector3/Functions.cc',
    'Sources/Vector3/Operations.cc'
]

module_mathematics_defines = [
    '-DCELL_MODULES_MATHEMATICS_AVAILABLE=1'
]

if host_machine.cpu_family() == 'aarch64'
    # TODO: acceleration support
elif host_machine.cpu_family() == 'x86_64'
    module_mathematics_defines += [
        '-DCELL_MODULES_MATHEMATICS_HARDWARE_ACCELERATE_X86=1',

        '-mfma'
    ]
endif

module_mathematics_library = shared_library('CellMathematics',
    sources: module_mathematics_sources,
    include_directories: module_mathematics_includes,
    dependencies: core
)

module_mathematics = declare_dependency(
    compile_args: module_mathematics_defines,
    include_directories: module_mathematics_includes,
    link_with: module_mathematics_library,
    dependencies: core
)

test('Vector', executable('CellMathematicsTestVector', sources: 'Tests/Vector.cc', dependencies: [ core, core_bootstrapper, module_mathematics ], win_subsystem: 'console'))
