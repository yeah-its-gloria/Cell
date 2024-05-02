# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

vulkan_dep = dependency('vulkan', required: true)

module_renderer_includes = [
    include_directories('Includes'),

    include_directories('Backends/Vulkan/Includes'),
]

module_renderer_sources = [
    'Sources/Stub.cc',

    # Vulkan

    'Backends/Vulkan/Sources/Buffer/Buffer.cc',
    'Backends/Vulkan/Sources/Buffer/Operations.cc',

    'Backends/Vulkan/Sources/CommandBuffer/CommandBuffer.cc',
    'Backends/Vulkan/Sources/CommandBuffer/Create.cc',
    'Backends/Vulkan/Sources/CommandBuffer/Submit.cc',
    'Backends/Vulkan/Sources/CommandBuffer/SubmitRender.cc',
    'Backends/Vulkan/Sources/CommandBuffer/Write.cc',

    'Backends/Vulkan/Sources/Device/CreateImageView.cc',
    'Backends/Vulkan/Sources/Device/Device.cc',

    'Backends/Vulkan/Sources/Image/Image.cc',
    'Backends/Vulkan/Sources/Image/CopyDataFromBuffer.cc',

    'Backends/Vulkan/Sources/Instance/Instance.cc',
    'Backends/Vulkan/Sources/Instance/QueryPhysicalDevice.cc',
    'Backends/Vulkan/Sources/Instance/ScorePhysicalDevice.cc',

    'Backends/Vulkan/Sources/Pipeline/AddMultiShader.cc',
    'Backends/Vulkan/Sources/Pipeline/AddResources.cc',
    'Backends/Vulkan/Sources/Pipeline/AddShader.cc',
    'Backends/Vulkan/Sources/Pipeline/Finalize.cc',
    'Backends/Vulkan/Sources/Pipeline/Pipeline.cc',

    'Backends/Vulkan/Sources/WSITarget/AcquireNext.cc',
    'Backends/Vulkan/Sources/WSITarget/Create.cc',
    'Backends/Vulkan/Sources/WSITarget/Present.cc',
    'Backends/Vulkan/Sources/WSITarget/Recreate.cc',
    'Backends/Vulkan/Sources/WSITarget/WSITarget.cc',

    'Backends/Vulkan/Sources/WSITarget/DeviceUtilities/CreateDepthStencilImage.cc',
    'Backends/Vulkan/Sources/WSITarget/DeviceUtilities/CreateSwapchainForSurface.cc',
    'Backends/Vulkan/Sources/WSITarget/DeviceUtilities/GetSurfaceData.cc'
]

module_renderer_defines = [
    '-DCELL_MODULES_RENDERER_AVAILABLE=1',

    '-DCELL_MODULES_RENDERER_VULKAN_AVAILABLE=1'
]

module_renderer_linker_args = [ ]

if host_machine.system() == 'windows'
    module_renderer_includes += include_directories('Backends/D3D12/Includes')

    module_renderer_sources += [
        'Backends/Vulkan/Sources/WSITarget/Platform/Windows.cc',

        'Backends/D3D12/Sources/CommandList.cc',
        'Backends/D3D12/Sources/Device.cc',
        'Backends/D3D12/Sources/Pipeline.cc',
        'Backends/D3D12/Sources/Resource.cc',
        'Backends/D3D12/Sources/Swapchain.cc'
    ]

    module_renderer_defines += [
        '-DCELL_MODULES_RENDERER_D3D12_AVAILABLE=1',
        '-Wno-language-extension-token'
    ]

    # BUG: Meson's Vulkan dependency check doesn't properly insert the library path
    module_renderer_linker_args += [ '-ld3d12', '-ldxgi', '-LC:/VulkanSDK/1.3.275.0/Lib' ]
elif host_machine.system() == 'darwin'
    module_renderer_includes += include_directories('Backends/Metal/Includes')

    module_renderer_sources += [
        'Backends/Vulkan/Sources/WSITarget/Platform/macOS.cc',

        'Backends/Metal/Sources/Device.mm'
    ]

    module_renderer_defines += '-DCELL_MODULES_RENDERER_METAL_AVAILABLE=1'

    module_renderer_linker_args += [
        '-Wl,-framework,CoreFoundation',
        '-Wl,-framework,CoreGraphics',
        '-Wl,-framework,Metal'
    ]
elif host_machine.system() == 'linux'
    module_renderer_sources += [
        'Backends/Vulkan/Sources/WSITarget/Platform/Linux.cc',
    ]
endif

module_renderer_library = shared_library('CellRenderer',
    sources: module_renderer_sources,
    include_directories: module_renderer_includes,
    link_args: module_renderer_linker_args,
    dependencies: [ core, module_mathematics, module_shell, vulkan_dep ]
)

module_renderer = declare_dependency(
    compile_args: module_renderer_defines,
    include_directories: module_renderer_includes,
    link_with: module_renderer_library,
    dependencies: [ core, module_mathematics, module_shell, vulkan_dep ]
)

if get_option('utilities')
    if host_machine.system() == 'windows'
        executable('CellRendererUtilitiesD3D12Runner',
            sources: [ 'Backends/D3D12/Utilities/Runner/Sources/Main.cc', 'Backends/D3D12/Utilities/Runner/Sources/Tools.cc', ],
            dependencies: [ core, core_bootstrapper, module_renderer ],
            win_subsystem: 'windows'
        )
    elif host_machine.system() == 'darwin'
        executable('CellRendererUtilitiesMetalRunner', sources: 'Backends/Metal/Utilities/Runner.cc', dependencies: [ core, core_bootstrapper, module_renderer ])
    endif
endif
