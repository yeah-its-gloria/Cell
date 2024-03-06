// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/Implementations/WASAPI.hh>
#include <Cell/System/Memory.hh>

#include <Cell/System/Log.hh>

namespace Cell::Audio::Implementations::WASAPI {

Capturer::~Capturer() {
    this->captureClient->Release();
    this->client->Release();
    this->device->Release();
}

Result Capturer::Start() {
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

Result Capturer::Stop() {
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

Result Capturer::Fetch(System::OwnedBlock<uint8_t>& out) {
    uint8_t* frameBuffer = nullptr;
    uint32_t frameCount = 0;
    DWORD flags = 0;

    uint32_t size = 0;
    HRESULT result = this->captureClient->GetNextPacketSize(&size);
    if (size == 0) {
        return Result::NoMoreSamples;
    }

    result = this->captureClient->GetBuffer(&frameBuffer, &frameCount, &flags, nullptr, nullptr);
    switch (result) {
    case S_OK: {
        break;
    }

    case AUDCLNT_E_BUFFER_OPERATION_PENDING: {
        return Result::TargetOccupied;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED: {
        return Result::TargetLost;
    }

    default: {
        System::Panic("IAudioCaptureClient::GetBuffer failed");
    }
    }

    out.Resize(frameCount * this->sampleSize);
    System::CopyMemory<uint8_t>(out, frameBuffer, frameCount * this->sampleSize);

    result = this->captureClient->ReleaseBuffer(frameCount);
    switch (result) {
    case S_OK: {
        break;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED: {
        return Result::TargetLost;
    }

    default: {
        System::Panic("IAudioCaptureClient::ReleaseBuffer failed");
    }
    }

    return Result::Success;
}

}
