// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/D3D12/Pipeline.hh>

namespace Cell::D3D12 {

Wrapped<Pipeline*, Result> Device::CreatePipeline(const Memory::IBlock& vertex, const Memory::IBlock& pixel) {
    const D3D12_BLEND_DESC blendStateInfo = {
        .AlphaToCoverageEnable = FALSE,
        .IndependentBlendEnable = FALSE,
        .RenderTarget[0] = {
            .BlendEnable = FALSE,
            .LogicOpEnable = FALSE,
            .SrcBlend = D3D12_BLEND_ZERO,
            .DestBlend = D3D12_BLEND_ZERO,
            .BlendOp = D3D12_BLEND_OP_ADD,
            .SrcBlendAlpha = D3D12_BLEND_ZERO,
            .DestBlendAlpha = D3D12_BLEND_ZERO,
            .BlendOpAlpha = D3D12_BLEND_OP_ADD,
            .LogicOp = D3D12_LOGIC_OP_SET,
            .RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL
        }
    };

    const D3D12_RASTERIZER_DESC rasterizerInfo = {
        .FillMode = D3D12_FILL_MODE_SOLID,
        .CullMode = D3D12_CULL_MODE_BACK,
        .FrontCounterClockwise = FALSE,
        .DepthBias = 0,
        .DepthBiasClamp = 1.f,
        .SlopeScaledDepthBias = 1.f,
        .DepthClipEnable = FALSE,
        .MultisampleEnable = FALSE,
        .AntialiasedLineEnable = FALSE,
        .ForcedSampleCount = 1,
        .ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
    };

    const D3D12_DEPTH_STENCIL_DESC depthStencilInfo = {
        .DepthEnable = FALSE,
        .DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO,
        .DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS,
        .StencilEnable = FALSE,
        .StencilReadMask = 0xff,
        .StencilWriteMask = 0xff,

        .FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP,
        .FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP,
        .FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP,
        .FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS,

        .BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP,
        .BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP,
        .BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP,
        .BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS
    };

    const D3D12_INPUT_ELEMENT_DESC inputInfo[2] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 3, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // TODO: specify this fully lol

#pragma clang diagnostic ignored "-Wmissing-field-initializers"
    const D3D12_GRAPHICS_PIPELINE_STATE_DESC info = {
        .pRootSignature = this->rootSignature,

        .VS.BytecodeLength  = vertex.GetSize(),
        .VS.pShaderBytecode = vertex.AsPointer(),

        .PS.BytecodeLength  = pixel.GetSize(),
        .PS.pShaderBytecode = pixel.AsPointer(),

        .BlendState = blendStateInfo,

        .SampleMask = UINT_MAX,

        .RasterizerState = rasterizerInfo,
        .DepthStencilState = depthStencilInfo,

        .InputLayout.pInputElementDescs = inputInfo,
        .InputLayout.NumElements = CELL_COUNT_OF(inputInfo),

        .PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,

        .NumRenderTargets = 1,
        .RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM,

        .SampleDesc.Count = 1
    };

    ID3D12PipelineState* state;
    const HRESULT result = this->device->CreateGraphicsPipelineState(&info, __uuidof(ID3D12PipelineState), (void**)&state);
    switch (result) {
    case S_OK: {
        break;
    }

    case E_OUTOFMEMORY: {
        return Result::NotEnoughMemory;
    }

    default: {
        System::Panic("ID3D12Device1::CreateRootSignature failed");
    }
    }

    return new Pipeline(this, state);
}

Pipeline::~Pipeline() {
    this->state->Release();
}

}
