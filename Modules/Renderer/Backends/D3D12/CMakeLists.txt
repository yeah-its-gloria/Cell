# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

set(submodule_include_dir ${CMAKE_CURRENT_LIST_DIR}/Includes)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${CMAKE_CURRENT_LIST_DIR}/BuildScripts)

include(D3D12Shader)

target_sources(CellRenderer PRIVATE
    ${submodule_include_dir}/Cell/Renderer/D3D12/CommandList.hh
    ${submodule_include_dir}/Cell/Renderer/D3D12/Device.hh
    ${submodule_include_dir}/Cell/Renderer/D3D12/Pipeline.hh
    ${submodule_include_dir}/Cell/Renderer/D3D12/Resource.hh
    ${submodule_include_dir}/Cell/Renderer/D3D12/Swapchain.hh

    ${CMAKE_CURRENT_LIST_DIR}/Sources/CommandList.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Device.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Pipeline.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Resource.cc
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Swapchain.cc
)

target_compile_definitions(CellRenderer PUBLIC "CELL_MODULES_RENDERER_D3D12_AVAILABLE=1")
target_compile_options(CellRenderer PRIVATE -Wno-language-extension-token)
target_include_directories(CellRenderer PUBLIC ${submodule_include_dir})
target_link_libraries(CellRenderer PRIVATE d3d12 dxgi)

if (CELL_ENABLE_UTILITIES)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Utilities/Runner)
endif ()
