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
friend class CommandList;
friend class Resource;

#if CELL_MODULES_OPENXR_AVAILABLE
friend class Cell::OpenXR::Instance;
friend class Cell::OpenXR::Session;
#endif

public:
    // Finds the most suitable device and initializes it.
    CELL_FUNCTION static Wrapped<Device*, Result> New();

    // Cleans up all resources and destroys this device.
    CELL_FUNCTION ~Device();

    // Creates a command list.
    CELL_FUNCTION Wrapped<class CommandList*, Result> CreateCommandList(class Pipeline* CELL_NONNULL pipeline);

    // Creates a new resource on the device.
    // Stride and size are expected in bytes. The size covers the full buffer.
    CELL_FUNCTION Wrapped<class Resource*, Result> CreateResource(const size_t stride, const size_t size);

    // Creates a pipeline with a vertex and pixel shader, using DXIL bytecode for both.
    CELL_FUNCTION Wrapped<class Pipeline*, Result> CreatePipeline(const IBlock& vertex, const IBlock& pixel);

    // Creates a swapchain with the Windows shell implementation.
    CELL_FUNCTION Wrapped<class Swapchain*, Result> CreateSwapchain(Shell::Implementations::Windows* CELL_NONNULL shell);

    // Wrapper for IShell to create a swapchain.
    CELL_FUNCTION_INTERNAL CELL_INLINE Wrapped<class Swapchain*, Result> CreateSwapchain(Shell::IShell* CELL_NONNULL shell) {
        return this->CreateSwapchain((Shell::Implementations::Windows*)shell);
    }

    CELL_NON_COPYABLE(Device)

private:
    CELL_FUNCTION_INTERNAL CELL_INLINE Device(IDXGIFactory2* CELL_NONNULL f,
                                              ID3D12Device1* CELL_NONNULL d,
                                              ID3D12CommandQueue* CELL_NONNULL q,
                                              ID3D12CommandAllocator* CELL_NONNULL a,
                                              ID3DBlob* CELL_NONNULL b,
                                              ID3D12RootSignature* CELL_NONNULL s)
        : factory(f), device(d), mainQueue(q), commandAllocator(a), rootSignatureBlob(b), rootSignature(s) { }

    IDXGIFactory2* factory;
    ID3D12Device1* device;
    ID3D12CommandQueue* mainQueue;
    ID3D12CommandAllocator* commandAllocator;
    ID3DBlob* rootSignatureBlob;
    ID3D12RootSignature* rootSignature;
};

}
