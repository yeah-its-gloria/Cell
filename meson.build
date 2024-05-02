# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project('Cell',
    'c', 'cpp',

    version: '1.0.0',

    default_options: [ 'c_std=c17', 'cpp_std=c++20' ],

    license: 'BSD 2-Clause',
    license_files: 'LICENSE'
)

if host_machine.system() == 'darwin'
    add_languages('objc', 'objcpp', required: true)
endif

# BUG: Some of these paths break on Windows without an extra slash

subdir('Core/')

subdir('Modules/DataManagement')
subdir('Modules/Mathematics')
subdir('Modules/Shell')
subdir('Modules/Audio')
subdir('Modules/Renderer/')

if host_machine.system() in [ 'windows', 'linux' ]
    subdir('Modules/OpenXR')
endif

if get_option('editor')
    subdir('Editor')
endif

if get_option('projects')
    subdir('Projects/AudioDuplicator')
    subdir('Projects/Example')
endif
