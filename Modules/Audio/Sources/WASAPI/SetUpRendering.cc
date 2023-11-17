// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/WASAPI.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Audio {

#define CHECK_RESULT if (result != Result::Success) { return result; }

Result WASAPI::SetUpRendering(Format format) {
    Result result = this->FindRenderDevice();
    CHECK_RESULT

    result = this->CreateRenderClient(format);
    CHECK_RESULT

    return Result::Success;
}

}
