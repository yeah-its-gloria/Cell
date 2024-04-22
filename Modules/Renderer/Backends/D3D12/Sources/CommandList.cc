// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Renderer/D3D12/CommandList.hh>
#include <Cell/Renderer/D3D12/Resource.hh>

namespace Cell::Renderer::D3D12 {

Wrapped<CommandList*, Result> Device::CreateCommandList(Pipeline* pipeline) {
    ID3D12GraphicsCommandList1* list = nullptr;
    HRESULT result = this->device->CreateCommandList(0,
                                                     D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                     this->commandAllocator,
                                                     pipeline->state,
                                                     __uuidof(ID3D12GraphicsCommandList1),
                                                     (void**)&list);

    switch (result) {
    case S_OK: {
        break;
    }

    case E_OUTOFMEMORY: {
        return Result::OutOfHostMemory;
    }

    default: {
        System::Panic("ID3D12Device1::CreateCommandList failed");
    }
    }

    result = list->Close();
    switch (result) {
    case S_OK: {
        break;
    }

    case E_OUTOFMEMORY: {
        return Result::OutOfHostMemory;
    }

    default: {
        System::Panic("ID3D12Device1::CreateCommandList failed");
    }
    }

    return new CommandList(this, pipeline, list);
}

void CommandList::StupidTestThing(Swapchain* swapchain, Resource* vertices) {
    this->list->Reset(this->device->commandAllocator, this->pipeline->state);

    this->list->SetGraphicsRootSignature(this->device->rootSignature);

    this->list->RSSetViewports(1, &swapchain->viewport);
    this->list->RSSetScissorRects(1, &swapchain->scissor);

    const D3D12_RESOURCE_BARRIER renderBarrier = {
        .Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
        .Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE,

        .Transition.pResource   = swapchain->resources[swapchain->index],
        .Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET,
        .Transition.StateAfter  = D3D12_RESOURCE_STATE_PRESENT,
        .Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
    };

    this->list->ResourceBarrier(1, &renderBarrier);

    const D3D12_CPU_DESCRIPTOR_HANDLE handle = swapchain->cpuHandles[swapchain->index];
    this->list->OMSetRenderTargets(1, &handle, FALSE, nullptr);

    const float clearColor[4] = { 0.f, 0.f, 0.f, 0.f };
    this->list->ClearRenderTargetView(handle, clearColor, 0, nullptr);

    this->list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    this->list->IASetVertexBuffers(0, 1, &vertices->view);

    this->list->DrawInstanced(3, 1, 0, 0);

    const D3D12_RESOURCE_BARRIER presentBarrier = {
        .Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
        .Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE,

        .Transition.pResource   = swapchain->resources[swapchain->index],
        .Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT,
        .Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET,
        .Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
    };

    this->list->ResourceBarrier(1, &presentBarrier);

    this->list->Close();

    swapchain->index = (swapchain->index + 1) % SWAPCHAIN_IMAGE_COUNT;

    this->device->mainQueue->ExecuteCommandLists(1, (ID3D12CommandList**)&this->list);
    swapchain->swapchain->Present(1, 0);
}

CommandList::~CommandList() {
    this->list->Release();
}

}
