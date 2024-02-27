// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/Implementations/WASAPI.hh>
#include <Cell/System/Memory.hh>

namespace Cell::Audio::Implementations::WASAPI {

Renderer::~Renderer() {
    this->renderClient->Release();
    this->client->Release();
    this->device->Release();
}

Result Renderer::Start() {
    const HRESULT result = this->client->Start();
    switch (result) {
    case S_OK:
    case AUDCLNT_E_NOT_STOPPED: {
        break;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED: {
        return Result::TargetLost;
    }

    default: {
        System::Panic("IAudioClient3::Start failed");
    }
    }

    return Result::Success;
}

Result Renderer::Stop() {
    const HRESULT result = this->client->Stop();
    switch (result) {
    case S_OK:
    case S_FALSE: {
        break;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED: {
        return Result::TargetLost;
    }

    default: {
        System::Panic("IAudioClient3::Stop failed");
    }
    }

    return Result::Success;
}

Result Renderer::Submit(const IBlock& block) {
    const size_t size = block.ByteSize();
    if (size % this->sampleSize != 0) {
        return Result::InvalidParameters;
    }

    uint8_t* frameBuffer = nullptr;
    HRESULT result = this->renderClient->GetBuffer(size / this->sampleSize, &frameBuffer);
    switch (result) {
    case S_OK: {
        break;
    }

    case AUDCLNT_E_BUFFER_SIZE_ERROR:
    case AUDCLNT_E_BUFFER_TOO_LARGE: {
        return Result::InvalidParameters;
    }

    case AUDCLNT_E_BUFFER_OPERATION_PENDING: {
        return Result::TargetOccupied;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED: {
        return Result::TargetLost;
    }

    default: {
        System::Panic("IAudioRenderClient::GetBuffer failed");
    }
    }

    System::CopyMemory<uint8_t>(frameBuffer, (uint8_t*)block.Pointer(), size);

    result = this->renderClient->ReleaseBuffer(size / this->sampleSize,0);
    switch (result) {
    case S_OK: {
        break;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED: {
        return Result::TargetLost;
    }

    default: {
        System::Panic("IAudioRenderClient::GetBuffer failed");
    }
    }

    return Result::Success;
}

Wrapped<uint32_t, Result> Renderer::GetMaxSampleCount() {
    uint32_t count = 0;
    const HRESULT result = this->client->GetBufferSize(&count);
    switch (result) {
    case S_OK: {
        break;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED: {
        return Result::TargetLost;
    }

    default: {
        System::Panic("IAudioClient3::GetBufferSize failed");
    }
    }

    return count;
}

Wrapped<uint32_t, Result> Renderer::GetCurrentSampleOffset() {
    uint32_t offset = 0;
    const HRESULT result = this->client->GetCurrentPadding(&offset);
    switch (result) {
    case S_OK: {
        break;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED: {
        return Result::TargetLost;
    }

    default: {
        System::Panic("IAudioClient3::GetCurrentPadding failed");
    }
    }

    return offset;
}

uint32_t Renderer::GetLatency() {
    return 1000000000.0;
}

}
