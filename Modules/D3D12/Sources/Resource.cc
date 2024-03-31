// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/D3D12/Resource.hh>

namespace Cell::D3D12 {

Wrapped<Resource*, Result> Device::CreateResource(const size_t stride, const size_t size) {
    CELL_ASSERT(size > 0);

    const D3D12_HEAP_PROPERTIES properties = {
        .Type                 = D3D12_HEAP_TYPE_UPLOAD,

        .CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
        .MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN, // TODO: allow specifying if it's CPU accessible or not (UBOs need this!)

        .CreationNodeMask     = 0,
        .VisibleNodeMask      = 0
    };

    const D3D12_RESOURCE_DESC description = {
        .Dimension          = D3D12_RESOURCE_DIMENSION_BUFFER,

        .Alignment          = 0,

        .Width              = size,
        .Height             = 1,
        .DepthOrArraySize   = 1,

        .MipLevels          = 1,
        .Format             = DXGI_FORMAT_UNKNOWN,
        .SampleDesc.Count   = 1,
        .SampleDesc.Quality = 0,

        .Layout             = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
        .Flags              = D3D12_RESOURCE_FLAG_NONE
    };

    ID3D12Resource* resource = nullptr;
    const HRESULT result = this->device->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE, &description, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, __uuidof(ID3D12Resource), (void**)&resource);
    switch (result) {
    case S_OK: {
        break;
    }

    case E_OUTOFMEMORY: {
        return Result::NotEnoughMemory;
    }

    default: {
        System::Panic("ID3D12Device1::CreateCommittedResource failed");
    }
    }

    const D3D12_VERTEX_BUFFER_VIEW view = {
        .BufferLocation = resource->GetGPUVirtualAddress(),
        .SizeInBytes    = (UINT)size,
        .StrideInBytes  = (UINT)stride
    };

    return new Resource(this, resource, view);
}

Result Resource::CopyData(const Memory::IBlock& data) {
    const D3D12_RANGE range = { 0, 0 };

    void* block = nullptr;
    const HRESULT result = this->resource->Map(0, &range, (void**)&block);
    switch (result) {
    case S_OK: {
        break;
    }

    case E_OUTOFMEMORY: {
        return Result::NotEnoughMemory;
    }

    default: {
        System::Panic("ID3D12Resource::Map failed");
    }
    }

    Memory::Copy(block, data.AsPointer(), data.GetSize());

    this->resource->Unmap(0, nullptr);
    return Result::Success;
}

Resource::~Resource() {
    this->resource->Release();
}

}
