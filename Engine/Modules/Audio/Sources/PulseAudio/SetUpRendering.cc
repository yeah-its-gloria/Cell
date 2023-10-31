// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Audio/PulseAudio.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Audio {

Result PulseAudio::SetUpRendering(Format format) {
    (void)(format);

    // TODO: how?

    return Result::Success;
}

}
