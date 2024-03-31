// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Tools.hh"

#include <Cell/Scoped.hh>
#include <Cell/D3D12/CommandList.hh>
#include <Cell/Memory/UnownedBlock.hh>
#include <Cell/System/Entry.hh>
#include <Cell/System/Thread.hh>

using namespace Cell;

struct Vertex {
    float position[3];
    float color[4];
};

CELL_FUNCTION_INTERNAL void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    ScopedObject<Shell::IShell> shell = Shell::CreateShell("Cell - D3D12").Unwrap();

    ScopedObject<D3D12::Device> device = D3D12::Device::New().Unwrap();
    ScopedObject<D3D12::Swapchain> swapchain = device->CreateSwapchain(shell).Unwrap();

    ScopedObject<D3D12::Pipeline> pipeline = D3D12ToolsLoadPipeline("Modules/D3D12/Utilities/Runner/Content/DefaultVertex.dxil",
                                                                    "Modules/D3D12/Utilities/Runner/Content/DefaultPixel.dxil",
                                                                    device);

    const Vertex triangle[3] = {
        { {    0.f,  0.25f, 0.f }, { 1.f, 0.f, 0.f, 1.f } },
        { {  0.25f, -0.25f, 0.f }, { 0.f, 1.f, 0.f, 1.f } },
        { { -0.25f, -0.25f, 0.f }, { 0.f, 0.f, 1.f, 1.f } }
    };

    ScopedObject<D3D12::Resource> vertexBuffer = device->CreateResource(sizeof(Vertex), sizeof(triangle)).Unwrap();

    const D3D12::Result result = vertexBuffer->CopyData(Memory::UnownedBlock { triangle, 3 });
    CELL_ASSERT(result == D3D12::Result::Success);

    ScopedObject<D3D12::CommandList> list = device->CreateCommandList(pipeline).Unwrap();

    while (true) {
        const Shell::Result result = shell->RunDispatch();
        if (result == Shell::Result::RequestedQuit) {
            break;
        }

        list->StupidTestThing(swapchain, vertexBuffer);

        CELL_ASSERT(result == Shell::Result::Success);
        System::Thread::Yield();
    }
}
