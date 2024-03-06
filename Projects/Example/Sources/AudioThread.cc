// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Example.hh"
#include "InputController.hh"

#include <Cell/Audio/Renderer.hh>
#include <Cell/IO/File.hh>
#include <Cell/Scoped.hh>
#include <Cell/System/Timer.hh>

#include <Cell/System/Log.hh>

using namespace Cell;
using namespace Cell::Audio;

void Example::AudioThread() {
    ScopedObject<ISubsystem> subsystem = CreateSubsystem("Cell").Unwrap();

    const uint32_t sampleSize = ((2 * 32) / 8);

    const Format format = {
        .type = FormatType::Float32PCM,
        .channels = 2,
        .rate = 48000
    };

    ScopedObject<IRenderer> renderer = subsystem->CreateRenderer(format).Unwrap();

    ScopedObject<IO::File> file = IO::File::Open(this->GetContentPath("/SoundEffects/Boing.bin")).Unwrap();

    const uint32_t size = file->GetSize().Unwrap();
    CELL_ASSERT(size % sampleSize == 0);

    const uint32_t count = renderer->GetMaxSampleCount().Unwrap();

    Result result = renderer->Start();
    CELL_ASSERT(result == Result::Success);

    uint32_t dataOffset = 0;
    while (this->shell->IsStillActive()) {
        if (!this->controller->TriggeredAudio() || dataOffset >= size) {
            dataOffset = 0;
            System::Thread::Yield();
            continue;
        }

        System::SleepPrecise(renderer->GetLatency());

        const uint32_t offset = renderer->GetCurrentSampleOffset().Unwrap();
        if (offset > 0) {
            System::Thread::Yield();
            continue;
        }

        const uint32_t framesToWrite = (uint32_t)[&count, &offset, &size, &dataOffset]{
            if (size - dataOffset < count * sampleSize) {
                return ((uint32_t)size - dataOffset) / sampleSize;
            }

            return count - offset;
        }();

        System::OwnedBlock<uint8_t> buffer(framesToWrite * sampleSize);
        IO::Result ioResult = file->Read(buffer, dataOffset, false);
        CELL_ASSERT(ioResult == IO::Result::Success);

        result = renderer->Submit(buffer);
        CELL_ASSERT(result == Result::Success);

        dataOffset += framesToWrite * sampleSize;
    }

    result = renderer->Stop();
    CELL_ASSERT(result == Result::Success);
}
