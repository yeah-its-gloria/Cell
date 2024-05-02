# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

# Base parameters

core_includes = [ include_directories('Includes'), include_directories('.') ]

core_sources = [
    'Sources/Cell.cc',

    'Sources/String/Actions.cc',
    'Sources/String/Checks.cc',
    'Sources/String/Constructors.cc',
    'Sources/String/Conversions.cc',
    'Sources/String/Format.cc',
    'Sources/String/Operators.cc',

    'Sources/System/Panic.cc'
]

core_defines = [
    '-DCELL_DEPRECATED=[[deprecated]]',

    '-DCELL_NODISCARD=[[nodiscard]]',
    '-DCELL_NORETURN=[[noreturn]]',

    '-DCELL_UNMANGLED=extern "C"',

    '-DCELL_NONNULL=_Nonnull',
    '-DCELL_NULLABLE=_Nullable',

    '-DCELL_FUNCTION_INTERNAL=__attribute__((visibility("hidden")))',
    '-DCELL_FUNCTION_TEMPLATE=__attribute__((visibility("hidden"))) inline',

    '-Wno-c99-designator',
    '-Wno-c99-extensions',
    '-Wno-nullability-extension',
    '-Wno-nullability-completeness',
    '-Wno-c++11-extensions',
    '-Wno-c++17-extensions',
    '-Wno-c++2b-extensions'
]

core_linker_args = [ '-nostdlib++' ]

core_private_linker_args = []

core_bootstrap_sources = []
core_bootstrap_link_args = []

# Options

if get_option('core_assert_mode') == 'external'
    core_defines += '-DCELL_CORE_USE_EXTERNAL_ASSERT=1'
elif get_option('core_assert_mode') == 'panic'
    core_defines += '-DCELL_CORE_USE_PANIC_ASSERT=1'
elif get_option('core_assert_mode') == 'skip'
    core_defines += '-DCELL_CORE_SKIP_ASSERT=1'
endif

# Per platform management

if host_machine.system() == 'windows'
    core_sources += [
        'Platform/Windows/String.cc',

        'Platform/Windows/IO/File/File.cc',
        'Platform/Windows/IO/File/CheckPath.cc',
        'Platform/Windows/IO/File/Delete.cc',
        'Platform/Windows/IO/File/Open.cc',
        'Platform/Windows/IO/File/ReadWrite.cc',
        'Platform/Windows/IO/File/SetWorkingDirectory.cc',

        'Platform/Windows/IO/Directory/Directory.cc',

        'Platform/Windows/IO/HID/HID.cc',
        'Platform/Windows/IO/HID/Open.cc',

        'Platform/Windows/IO/Pipe/Pipe.cc',
        'Platform/Windows/IO/Pipe/ClientManagement.cc',
        'Platform/Windows/IO/Pipe/Connect.cc',
        'Platform/Windows/IO/Pipe/Create.cc',
        'Platform/Windows/IO/Pipe/WaitUntilReady.cc',

        'Platform/Windows/IO/USB/Internal.hh',
        'Platform/Windows/IO/USB/USB.cc',
        'Platform/Windows/IO/USB/Open.cc',

        'Platform/Windows/Memory/Allocator.cc',

        'Platform/Windows/Network/Internal.hh',
        'Platform/Windows/Network/AddressInfo.cc',
        'Platform/Windows/Network/TypeConversion.cc',
        'Platform/Windows/Network/Socket/Communication.cc',
        'Platform/Windows/Network/Socket/Connection.cc',
        'Platform/Windows/Network/Socket/NewDestruct.cc',

        'Platform/Windows/System/DynamicLibrary.cc',
        'Platform/Windows/System/Event.cc',
        'Platform/Windows/System/Log.cc',
        'Platform/Windows/System/Mutex.cc',
        'Platform/Windows/System/Panic.cc',
        'Platform/Windows/System/RNG.cc',
        'Platform/Windows/System/Thread.cc',
        'Platform/Windows/System/Timer.cc'
    ]

    # TODO: add resource scripts
    core_bootstrap_sources += [
        'Platform/Windows/Bootstrap.cc'
    ]

    core_defines += [
        '-DCELL_PLATFORM_WINDOWS=1',

        '-D_CRT_SECURE_NO_WARNINGS=1'
    ]

    if get_option('core_windows_log_console')
        core_defines += '-DCELL_PLATFORM_WINDOWS_LOG_TO_CONSOLE=1'
    endif

    core_linker_args += [
        '-lkernel32',
        '-lruntimeobject',
        '-luserenv',
        '-luser32',
        '-lws2_32'
    ]

    core_private_linker_args += [
        '-lbcrypt',
        '-lhid',
        '-lntdll',
        '-lsetupapi',
        '-lshlwapi',
        '-lwinusb'
    ]

    core_bootstrap_link_args += [
        '-lntdll',
        '-Wl,-entry:mainCRTStartup'
    ]
elif host_machine.system() == 'darwin'
    core_sources += [
        'Platform/macOS/VirtualStub.cc',

        'Platform/macOS/String.mm',

        'Platform/macOS/IO/File/File.mm',
        'Platform/macOS/IO/File/CheckPathSetWorkingDirectory.mm',
        'Platform/macOS/IO/File/OpenCreate.mm',
        'Platform/macOS/IO/File/ReadWriteFlush.mm',

        'Platform/macOS/IO/Directory.cc',
        'Platform/macOS/IO/HID.cc',
        'Platform/macOS/IO/Pipe.cc',
        'Platform/macOS/IO/USB.cc',

        'Platform/macOS/Memory/Allocator.cc',

        'Platform/macOS/Network/Internal.hh',
        'Platform/macOS/Network/AddressInfo.cc',
        'Platform/macOS/Network/TypeConversion.cc',
        'Platform/macOS/Network/Socket/NewDestruct.cc',
        'Platform/macOS/Network/Socket/Socket.cc',

        'Platform/macOS/System/DynamicLibrary.cc',
        'Platform/macOS/System/Event.cc',
        'Platform/macOS/System/Log.cc',
        'Platform/macOS/System/Mutex.mm',
        'Platform/macOS/System/Panic.mm',
        'Platform/macOS/System/Thread.mm',
        'Platform/macOS/System/Timer.cc'
    ]

    core_bootstrap_sources += 'Platform/macOS/Bootstrap.mm'

    core_defines += [
        '-DCELL_PLATFORM_MACOS=1'
    ]

    core_private_linker_args += [
        '-Wl,-framework,Foundation',
        '-Wl,-framework,AppKit',
        '-Wl,-framework,IOKit'
    ]

    core_bootstrap_link_args += [
        '-Wl,-framework,Foundation',
        '-Wl,-framework,AppKit'
    ]

    install_data('BuildContent/macOS/Cell.icns',  install_dir: 'Contents/Resources')
    install_data('BuildContent/macOS/Info.plist', install_dir: 'Contents')
elif host_machine.system() == 'linux'
    core_sources += [
        'Platform/Linux/VirtualStub.cc',

        'Platform/Linux/String.cc',

        'Platform/Linux/IO/File/File.cc',
        'Platform/Linux/IO/File/CheckPath.cc',
        'Platform/Linux/IO/File/Delete.cc',
        'Platform/Linux/IO/File/Offset.cc',
        'Platform/Linux/IO/File/Open.cc',
        'Platform/Linux/IO/File/ReadWrite.cc',
        'Platform/Linux/IO/File/SetWorkingDirectory.cc',

        'Platform/Linux/IO/Directory.cc',
        'Platform/Linux/IO/HID.cc',
        'Platform/Linux/IO/Pipe.cc',
        'Platform/Linux/IO/USB.cc',

        'Platform/Linux/Memory/Allocator.cc',

        'Platform/Linux/Network/Internal.hh',
        'Platform/Linux/Network/AddressInfo.cc',
        'Platform/Linux/Network/TypeConversion.cc',
        'Platform/Linux/Network/Socket/NewDestruct.cc',
        'Platform/Linux/Network/Socket/Socket.cc',

        'Platform/Linux/System/DynamicLibrary.cc',
        'Platform/Linux/System/Event.cc',
        'Platform/Linux/System/Log.cc',
        'Platform/Linux/System/Mutex.cc',
        'Platform/Linux/System/Panic.cc',
        'Platform/Linux/System/Thread.cc',
        'Platform/Linux/System/Timer.cc'
    ]

    core_bootstrap_sources += 'Platform/Linux/Bootstrap.cc'

    core_defines += [
        '-DCELL_PLATFORM_LINUX=1',

        '-D_FILE_OFFSET_BITS=64',
        '-D_REENTRANT=1'
    ]
endif

if host_machine.system() == 'windows'
    core_defines += '-DCELL_FUNCTION=__declspec(dllexport)'
else
    core_defines += '-DCELL_FUNCTION=__attribute__((visibility("default")))'
endif

if host_machine.system() == 'darwin'
    core_linker_args += '-Wl,-no_warn_duplicate_libraries'
else
    core_linker_args += '-fuse-ld=lld'
endif

# File generation

version_parts = meson.project_version().split('.')
assert(version_parts.length() == 3, 'Malformed version')

configure_file(
    input: 'GeneratedIncludes/CellVersion.h.in',
    output: 'CellVersion.h',
    configuration: {
        'Cell_VERSION_MAJOR': version_parts[0],
        'Cell_VERSION_MINOR': version_parts[1],
        'Cell_VERSION_PATCH': version_parts[2]
    }
)

# Target setup

core_library = shared_library('CellCore',
    sources: core_sources,
    include_directories: core_includes,

    c_args: core_defines,
    objc_args: core_defines,

    cpp_args: core_defines,
    objcpp_args: core_defines,

    link_args: [ core_linker_args, core_private_linker_args ]
)

core = declare_dependency(
    compile_args: core_defines,
    include_directories: core_includes,
    link_args: core_linker_args,
    link_with: core_library
)

core_bootstrapper = declare_dependency(sources: core_bootstrap_sources, link_args: core_bootstrap_link_args)

# Tests

if get_option('test_mode') in [ 'functioning', 'all' ]
    test('Enumerables', executable('CellCoreTestEnumerables', sources: 'Tests/Enumerables.cc', dependencies: [ core, core_bootstrapper ], win_subsystem: 'console'))
    test('Network',     executable('CellCoreTestNetwork',     sources: 'Tests/Network.cc',     dependencies: [ core, core_bootstrapper ], win_subsystem: 'console'))
    test('String',      executable('CellCoreTestString',      sources: 'Tests/String.cc',      dependencies: [ core, core_bootstrapper ], win_subsystem: 'console'))
    test('System',      executable('CellCoreTestSystem',      sources: 'Tests/System.cc',      dependencies: [ core, core_bootstrapper ], win_subsystem: 'console'))
    test('Utilities',   executable('CellCoreTestUtilities',   sources: 'Tests/Utilities.cc',   dependencies: [ core, core_bootstrapper ], win_subsystem: 'console'))

    if host_machine.system() == 'windows' and get_option('test_mode') == 'all'
        test('WindowsDirectoryInspection', executable('CellCoreTestWindowsDirectoryInspection', sources: 'Tests/WindowsDirectoryInspection.cc', dependencies: [ core, core_bootstrapper ], win_subsystem: 'console'))
    endif
endif

