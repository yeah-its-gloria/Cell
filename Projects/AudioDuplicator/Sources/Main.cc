// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Audio/Capturer.hh>
#include <Cell/Audio/Renderer.hh>
#include <Cell/Shell/Shell.hh>
#include <Cell/System/Entry.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Timer.hh>

using namespace Cell;

void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    const Audio::Format format = {
        .type = Audio::FormatType::Float32PCM,
        .channels = 2,
        .rate = 48000
    };

    ScopedObject<Shell::IShell> shell = Shell::CreateShell("Cell - AudioDuplicator").Unwrap();

    ScopedObject<Audio::ISubsystem> subsystem = Audio::CreateSubsystem("Cell - AudioDuplicator").Unwrap();

    Collection::List<Audio::DeviceInfo> renderers = subsystem->DiscoverAvailableRenderers().Unwrap();

    // THIS IS VERY HARDCODED
    // SORRY :(

    shell->Log(String::Format("Loopback:\t%", renderers[3].name));
    shell->Log(String::Format("Render:\t\t%", renderers[0].name));

    ScopedObject<Audio::ICapturer> loopback = subsystem->CreateLoopback(renderers[3], format).Unwrap();
    ScopedObject<Audio::IRenderer> renderer = subsystem->CreateRenderer(renderers[0], format).Unwrap();

    Audio::Result result = loopback->Start();
    CELL_ASSERT(result == Audio::Result::Success);

    result = renderer->Start();
    CELL_ASSERT(result == Audio::Result::Success);

    while (true) {
        Shell::Result shellResult = shell->RunDispatch();
        if (shellResult == Shell::Result::RequestedQuit) {
            break;
        }

        CELL_ASSERT(shellResult == Cell::Shell::Result::Success);

        System::SleepPrecise(renderer->GetLatency());

        System::OwnedBlock<uint8_t> buffer(1);
        result = loopback->Fetch(buffer);
        if (result == Audio::Result::NoMoreSamples) {
            continue;
        }

        CELL_ASSERT(result == Audio::Result::Success);

        result = renderer->Submit(buffer);
        CELL_ASSERT(result == Audio::Result::Success);
    }

    result = renderer->Stop();
    CELL_ASSERT(result == Audio::Result::Success);

    result = loopback->Stop();
    CELL_ASSERT(result == Audio::Result::Success);
}
