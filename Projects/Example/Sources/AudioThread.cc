// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Example.hh"
#include "InputController.hh"

#include <Cell/Scoped.hh>
#include <Cell/Audio/Renderer.hh>
#include <Cell/IO/File.hh>
#include <Cell/Memory/OwnedBlock.hh>
#include <Cell/System/Timer.hh>

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

    const uint32_t size = file->GetSize();
    CELL_ASSERT(size % sampleSize == 0);

    const uint32_t count = renderer->GetMaxSampleCount().Unwrap();

    Result result = renderer->Start();
    CELL_ASSERT(result == Result::Success);

    // BUG: CoreAudio hates this design, right now this doesn't track the samples played properly
    // TODO: build a better way of even doing buffers in the first place
    uint32_t dataOffset = 0;
    while (this->shell->IsStillActive()) {
        if (!this->controller->TriggeredAudio() || dataOffset >= size) {
            dataOffset = 0;
            System::SleepPrecise(renderer->GetLatency());
            continue;
        }

        System::SleepPrecise(renderer->GetLatency());

        // BUG: this doesn't function well on PulseAudio with particularly small samples
        const uint32_t offset = renderer->GetCurrentSampleOffset().Unwrap();
        if (offset > 0) {
            System::SleepPrecise(renderer->GetLatency());
            continue;
        }

        const uint32_t framesToWrite = (uint32_t)[&count, &size, &dataOffset]{
            if (size - dataOffset < count * sampleSize) {
                return ((uint32_t)size - dataOffset) / sampleSize;
            }

            return count;
        }();

        Memory::OwnedBlock<uint8_t> buffer(framesToWrite * sampleSize);
        IO::Result ioResult = file->Read(buffer, dataOffset);
        CELL_ASSERT(ioResult == IO::Result::Success);

        result = renderer->Submit(buffer);
        CELL_ASSERT(result == Result::Success);

        dataOffset += framesToWrite * sampleSize;
    }

    result = renderer->Stop();
    CELL_ASSERT(result == Result::Success);
}
