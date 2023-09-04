// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/WASAPI.hh>
#include <Cell/System/Memory.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Audio {

Result WASAPI::WriteSamples(const uint8_t* CELL_NONNULL frames, const uint32_t count, const bool treatAsSilent) {
    if (count == 0) {
        return Result::InvalidParameters;
    }

    uint8_t* frameBuffer = nullptr;
    HRESULT result = this->renderClientService->GetBuffer(count, &frameBuffer);
    switch (result) {
    case S_OK: {
        break;
    }

    case AUDCLNT_E_BUFFER_SIZE_ERROR:
    case AUDCLNT_E_BUFFER_TOO_LARGE: {
        return Result::InvalidParameters;
    }

    case AUDCLNT_E_BUFFER_OPERATION_PENDING: {
        return Result::DeviceOccupied;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED: {
        return Result::DeviceLost;
    }

    default: {
        System::Panic("IAudioRenderClient::GetBuffer failed");
    }
    }

    System::CopyMemory<uint8_t>(frameBuffer, frames, this->sampleSize * count);

    result = this->renderClientService->ReleaseBuffer(count, treatAsSilent ? AUDCLNT_BUFFERFLAGS_SILENT : 0); // TODO: allow writing silent samples
    switch (result) {
    case S_OK: {
        break;
    }

    case AUDCLNT_E_DEVICE_INVALIDATED: {
        return Result::DeviceLost;
    }

    default: {
        System::Panic("IAudioRenderClient::GetBuffer failed");
    }
    }

    return Result::Success;
}

}
