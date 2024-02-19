// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/D3D12/Pipeline.hh>
#include <Cell/D3D12/Swapchain.hh>

namespace Cell::D3D12 {

Wrapped<Device*, Result> Device::New() {
    IDXGIFactory2* factory = nullptr;
    HRESULT result = CreateDXGIFactory2(0, __uuidof(IDXGIFactory2), (void**)&factory);
    switch (result) {
    case S_OK: {
        break;
    }

    case E_NOTIMPL: {
        return Result::Unsupported;
    }

    case E_OUTOFMEMORY: {
        return Result::NotEnoughMemory;
    }

    default: {
        System::Panic("CreateDXGIFactory2 failed");
    }
    }

    ID3D12Device1* device = nullptr;
    result = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device1), (void**)&device);
    switch (result) {
    case S_OK: {
        break;
    }

    case E_NOTIMPL: {
        factory->Release();
        return Result::Unsupported;
    }

    case E_OUTOFMEMORY: {
        factory->Release();
        return Result::NotEnoughMemory;
    }

    default: {
        System::Panic("D3D12CreateDevice failed");
    }
    }

    const D3D12_COMMAND_QUEUE_DESC queueDescription = {
        .Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
        .Priority = 0,
        .Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
        .NodeMask = 0
    };

    ID3D12CommandQueue* mainQueue = nullptr;
    result = device->CreateCommandQueue(&queueDescription, __uuidof(ID3D12CommandQueue), (void**)&mainQueue);
    switch (result) {
    case S_OK: {
        break;
    }

    case E_OUTOFMEMORY: {
        device->Release();
        factory->Release();
        return Result::NotEnoughMemory;
    }

    default: {
        System::Panic("ID3D12Device1::CreateCommandQueue failed");
    }
    }

    ID3D12CommandAllocator* allocator = nullptr;
    result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&allocator);
    switch (result) {
    case S_OK: {
        break;
    }

    case E_OUTOFMEMORY: {
        device->Release();
        factory->Release();
        return Result::NotEnoughMemory;
    }

    default: {
        System::Panic("ID3D12Device1::CreateCommandAllocator failed");
    }
    }

    const D3D12_ROOT_SIGNATURE_DESC rootSigDesc = {
        .NumParameters = 0,
        .pParameters = nullptr,
        .NumStaticSamplers = 0,
        .pStaticSamplers = nullptr,
        .Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
    };

    ID3DBlob* rootBlob = nullptr;
    result = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootBlob, nullptr);
    switch (result) {
    case S_OK: {
        break;
    }

    case E_OUTOFMEMORY: {
        allocator->Release();
        mainQueue->Release();
        device->Release();
        factory->Release();
        return Result::NotEnoughMemory;
    }

    case E_NOTIMPL: {
        allocator->Release();
        mainQueue->Release();
        device->Release();
        factory->Release();
        return Result::Unsupported;
    }

    default: {
        System::Panic("D3D12SerializeRootSignature failed");
    }
    }

    ID3D12RootSignature* rootSig = nullptr;
    device->CreateRootSignature(0, rootBlob->GetBufferPointer(), rootBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&rootSig);
    switch (result) {
    case S_OK: {
        break;
    }

    case E_OUTOFMEMORY: {
        rootBlob->Release();
        allocator->Release();
        mainQueue->Release();
        device->Release();
        factory->Release();
        return Result::NotEnoughMemory;
    }

    default: {
        System::Panic("ID3D12Device1::CreateRootSignature failed");
    }
    }

    return new Device(factory, device, mainQueue, allocator, rootBlob, rootSig);
}

Device::~Device() {
    this->rootSignature->Release();
    this->rootSignatureBlob->Release();
    this->commandAllocator->Release();
    this->mainQueue->Release();
    this->device->Release();
    this->factory->Release();
}

}
