// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/D3D12/Result.hh>
#include <Cell/Shell/Implementations/Windows.hh>

#include <d3d12.h>
#include <dxgi1_5.h>

namespace Cell::D3D12 {

// Represents a D3D12 rendering device.
class Device : public Object {
public:
    // Finds the most suitable device and initializes it.
    CELL_FUNCTION static Wrapped<Device*, Result> New();

    // Cleans up all resources and destructs this device.
    CELL_FUNCTION ~Device();

    // Creates a swapchain with the Windows shell implementation.
    CELL_FUNCTION Result CreateSwapchain(Shell::Implementations::Windows* CELL_NONNULL shell);

    // Wrapper for IShell to create a swapchain.
    CELL_INLINE Result CreateSwapchain(Shell::IShell* CELL_NONNULL shell) {
        return this->CreateSwapchain((Shell::Implementations::Windows*)shell);
    }

private:
    CELL_INLINE Device(IDXGIFactory2* CELL_NONNULL factory, ID3D12Device1* CELL_NONNULL device, ID3D12CommandQueue* CELL_NONNULL mainQueue)
                        : factory(factory), device(device), mainQueue(mainQueue) { }

    IDXGIFactory2* factory;
    ID3D12Device1* device;
    ID3D12CommandQueue* mainQueue;

    IDXGISwapChain1* swapchain = nullptr;
};

}
