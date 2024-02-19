// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/D3D12/Swapchain.hh>

namespace Cell::D3D12 {

Wrapped<Swapchain*, Result> Device::CreateSwapchain(Shell::Implementations::Windows* shell) {
    const Shell::Extent extent = shell->GetDrawableExtent().Unwrap();

    const DXGI_SWAP_CHAIN_DESC1 swapchainInfo = {
        .Width              = extent.width,
        .Height             = extent.height,

        .Format             = DXGI_FORMAT_R8G8B8A8_UNORM,

        .Stereo             = FALSE,

        .SampleDesc.Count   = 1,
        .SampleDesc.Quality = 0,

        .BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount        = SWAPCHAIN_IMAGE_COUNT,

        .Scaling            = DXGI_SCALING_NONE,

        .SwapEffect         = DXGI_SWAP_EFFECT_FLIP_DISCARD,

        .AlphaMode          = DXGI_ALPHA_MODE_UNSPECIFIED,

        .Flags              = 0
    };

    IDXGISwapChain1* swapchain = nullptr;
    HRESULT result = this->factory->CreateSwapChainForHwnd(this->mainQueue, shell->GetWindow(), &swapchainInfo, nullptr, nullptr, &swapchain);
    switch (result) {
    case S_OK: {
        break;
    }

    case E_OUTOFMEMORY: {
        return Result::NotEnoughMemory;
    }

    case DXGI_ERROR_INVALID_CALL: {
        return Result::Unsupported;
    }

    default: {
        System::Panic("IDXGIFactory4::CreateSwapChainForHwnd failed");
    }
    }

    const D3D12_DESCRIPTOR_HEAP_DESC swapchainRenderHeapInfo = {
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
        .NumDescriptors = SWAPCHAIN_IMAGE_COUNT,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
        .NodeMask = 0,
    };

    ID3D12DescriptorHeap* swapchainRenderHeap = nullptr;
    result = this->device->CreateDescriptorHeap(&swapchainRenderHeapInfo, __uuidof(ID3D12DescriptorHeap), (void**)&swapchainRenderHeap);
    switch (result) {
    case S_OK: {
        break;
    }

    case E_OUTOFMEMORY: {
        return Result::NotEnoughMemory;
    }

    default: {
        System::Panic("ID3D12Device1::CreateDescriptorHeap failed");
    }
    }

    const UINT offset = this->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    ID3D12Resource* swapchainResources[SWAPCHAIN_IMAGE_COUNT];
    D3D12_CPU_DESCRIPTOR_HANDLE swapchainCPUHandles[SWAPCHAIN_IMAGE_COUNT];
    for (UINT i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++) {
        result =  swapchain->GetBuffer(i, __uuidof(ID3D12Resource), (void**)&(swapchainResources[i]));
        switch (result) {
        case S_OK: {
            break;
        }

        default: {
            System::Panic("IDXGISwapChain1::GetBuffer failed");
        }
        }

        swapchainCPUHandles[i] = swapchainRenderHeap->GetCPUDescriptorHandleForHeapStart();
        swapchainCPUHandles[i].ptr += offset * i;

        this->device->CreateRenderTargetView(swapchainResources[i], nullptr, swapchainCPUHandles[i]);
    }

    const D3D12_VIEWPORT viewport = {
        .TopLeftX = 0,
        .TopLeftY = 0,
        .Width    = (float)extent.width,
        .Height   = (float)extent.height,
        .MinDepth = 0.f,
        .MaxDepth = 1.f
    };

    const D3D12_RECT scissor = {
        .left     = 0,
        .top      = 0,
        .right    = (LONG)extent.width,
        .bottom   = (LONG)extent.height,
    };

    return new Swapchain(this, swapchain, swapchainResources, swapchainCPUHandles, viewport, scissor);
}

Swapchain::~Swapchain() {
    for (ID3D12Resource*& resource : this->resources) {
        resource->Release();
    }

    this->swapchain->Release();
}

}
