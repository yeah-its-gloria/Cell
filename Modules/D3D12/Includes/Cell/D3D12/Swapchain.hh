// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/D3D12/Device.hh>

namespace Cell::D3D12 {

const static UINT SWAPCHAIN_IMAGE_COUNT = 3;

// Implements a 3 image backed swapchain for regular windows.
class Swapchain : public Object {
friend class CommandList;
friend Device;

public:
    // Destroys the swapchain instance.
    CELL_FUNCTION ~Swapchain();

    CELL_NON_COPYABLE(Swapchain)

private:
    CELL_FUNCTION_INTERNAL CELL_INLINE Swapchain(Device* CELL_NONNULL d,
                                                 IDXGISwapChain1* CELL_NONNULL s,
                                                 ID3D12Resource* CELL_NONNULL r[SWAPCHAIN_IMAGE_COUNT],
                                                 D3D12_CPU_DESCRIPTOR_HANDLE c[SWAPCHAIN_IMAGE_COUNT],
                                                 D3D12_VIEWPORT v,
                                                 D3D12_RECT sc)
        : device(d), swapchain(s), resources { r[0], r[1], r[2] }, cpuHandles { c[0], c[1], c[2] }, viewport(v), scissor(sc) { }

    Device* device;

    IDXGISwapChain1* swapchain;
    ID3D12Resource* resources[SWAPCHAIN_IMAGE_COUNT];
    D3D12_CPU_DESCRIPTOR_HANDLE cpuHandles[SWAPCHAIN_IMAGE_COUNT];

    D3D12_VIEWPORT viewport;
    D3D12_RECT scissor;

    UINT index = 0;
};

}
