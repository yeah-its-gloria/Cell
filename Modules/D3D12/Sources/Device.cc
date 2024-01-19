// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/D3D12/Device.hh>

namespace Cell::D3D12 {

Wrapped<Device*, Result> Device::New() {
    IDXGIFactory2* factory = nullptr;
    HRESULT result = CreateDXGIFactory2(0, __uuidof(IDXGIFactory2), (void**)&factory);
    switch (result) {
    case S_OK: {
        break;
    }

    default: {
        System::Panic("CreateDXGIFactory1 failed");
    }
    }

    ID3D12Device1* device = nullptr;
    result = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device1), (void**)&device);
    switch (result) {
    case S_OK: {
        break;
    }

    default: {
        System::Panic("D3D12CreateDevice failed");
    }
    }

    const D3D12_COMMAND_QUEUE_DESC queueDescription = {
        .Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
        .Priority = 0,
        .Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
        .NodeMask = 0};

    ID3D12CommandQueue* mainQueue = nullptr;
    result = device->CreateCommandQueue(&queueDescription, __uuidof(ID3D12CommandQueue), (void**)&mainQueue);
    switch (result) {
    case S_OK: {
        break;
    }

    default: {
        System::Panic("ID3D12CreateDevice1::CreateCommandQueue failed");
    }
    }

    return new Device(factory, device, mainQueue);
}

Device::~Device() {
    ULONG refCount = 0;

    if (this->swapchain != nullptr) {
        refCount = this->swapchain->Release();
        CELL_ASSERT(refCount == 0);
    }

    refCount = this->mainQueue->Release();
    CELL_ASSERT(refCount == 0);

    refCount = this->device->Release();
    CELL_ASSERT(refCount == 0);

    refCount = this->factory->Release();
    CELL_ASSERT(refCount == 0);
}

Result Device::CreateSwapchain(Shell::Implementations::Windows* shell) {
    const Shell::Extent extent = shell->GetDrawableExtent().Unwrap();

    DXGI_SWAP_CHAIN_DESC1 swapchainDescription = {
        .Width = extent.width,
        .Height = extent.height,
        .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
        .Stereo = FALSE,
        .SampleDesc.Count = 1,
        .SampleDesc.Quality = 0,
        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount = 3,
        .Scaling = DXGI_SCALING_NONE,
        .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
        .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
        .Flags = 0
    };

    HRESULT result = this->factory->CreateSwapChainForHwnd(this->mainQueue, shell->GetWindow(), &swapchainDescription, nullptr, nullptr, &this->swapchain);
    switch (result) {
    case S_OK: {
        break;
    }

    case DXGI_ERROR_INVALID_CALL: {
        return Result::Unsupported;
    }

    default: {
        System::Panic("IDXGIFactory4::CreateSwapChainForHwnd failed");
    }
    }

    return Result::Success;
}

}
