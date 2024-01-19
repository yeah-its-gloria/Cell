// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Example.hh"

#include <Cell/Audio/Engine.hh>
#include <Cell/IO/File.hh>
#include <Cell/Scoped.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Timer.hh>
#include <Cell/System/Thread.hh>

using namespace Cell;
using namespace Cell::Audio;

void Example::AudioThread() {
    ScopedObject instance = CreateEngine().Unwrap();

    Format format = {
        .type = FormatType::Float32PCM,
        .channels = 2,
        .rate = 48000
    };

    Result result = instance->SetUpRendering(format);
    CELL_ASSERT(result == Result::Success);

    ScopedObject file = IO::File::Open(this->GetContentPath("/Sounds/boing.bin")).Unwrap();

    const size_t size = file->GetSize().Unwrap();
    CELL_ASSERT(size % 4 == 0);

    const uint32_t count = instance->GetBufferSize().Unwrap();

    result = instance->PlaybackBegin();
    CELL_ASSERT(result == Result::Success);

    const uint32_t sampleSize = ((2 * 32) / 8);
    System::OwnedBlock<uint8_t> buffer(count * sampleSize);

    // TODO: short samples get played twice, fix that

    uint32_t dataOffset = 0;
    uint32_t framesToWrite = 0;
    while (this->shell->IsStillActive()) {
        if (this->audioTrigger.IsDataAvailable()) {
            while (this->shell->IsStillActive()) {
                System::SleepPrecise(instance->GetLatencyMicroseconds());

                // prevents stupidly small sample sizes from underflowing us
                if (dataOffset >= size) {
                    break;
                }

                const uint32_t offset = instance->GetCurrentBufferFillCount().Unwrap();
                if (offset > 0) {
                    System::Thread::Yield();
                    continue;
                }

                framesToWrite = count - offset;
                CELL_ASSERT(framesToWrite > 0);

                if (size - dataOffset < framesToWrite * sampleSize) {
                    framesToWrite = (size - dataOffset) / sampleSize;
                }

                if (buffer.Count() != framesToWrite * sampleSize) {
                    buffer.Resize(framesToWrite * sampleSize);
                }

                IO::Result ioResult = file->Read(buffer, dataOffset);
                CELL_ASSERT(ioResult == IO::Result::Success);

                result = instance->WriteSamples(buffer, framesToWrite);
                CELL_ASSERT(result == Result::Success);

                dataOffset += framesToWrite * sampleSize;
            }

            dataOffset = 0;
            audioTrigger.Read();
        }

        System::Thread::Yield();
    }

    result = instance->PlaybackEnd();
    CELL_ASSERT(result == Result::Success || result == Result::NotYetFinished);
}
