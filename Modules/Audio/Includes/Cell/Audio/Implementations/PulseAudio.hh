// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Audio/Renderer.hh>

#include <pulse/pulseaudio.h>

namespace Cell::Audio::Implementations::PulseAudio {

// PulseAudio subsystem implementation.
class Subsystem : public ISubsystem {
friend class Renderer;

public:
    CELL_FUNCTION static Wrapped<Subsystem*, Result> New(const System::String& title);
    CELL_FUNCTION ~Subsystem() override;
    CELL_FUNCTION Wrapped<IRenderer*, Result> CreateRenderer(Format format) override;

private:
    CELL_INLINE Subsystem(pa_threaded_mainloop* l, pa_context* c) : loop(l), context(c) { };

    CELL_FUNCTION_INTERNAL static void StateCallback(pa_context* context, void* userdata);

    pa_threaded_mainloop* loop;
    pa_context* context;
    pa_context_state_t contextState = PA_CONTEXT_UNCONNECTED;
};

// PulseAudio renderer implementation.
class Renderer : public IRenderer {
friend Subsystem;

public:
    CELL_FUNCTION ~Renderer() override;
    CELL_FUNCTION Result Start() override;
    CELL_FUNCTION Result Stop() override;
    CELL_FUNCTION Result Submit(const IBlock& block) override;
    CELL_FUNCTION Wrapped<uint32_t, Result> GetMaxSampleCount() override;
    CELL_FUNCTION Wrapped<uint32_t, Result> GetCurrentSampleOffset() override;
    CELL_FUNCTION uint32_t GetLatency() override;

private:
    CELL_INLINE Renderer(Subsystem* s, pa_stream* st, size_t ss) : subsystem(s), stream(st), sampleSize(ss) { }

    CELL_FUNCTION_INTERNAL static void StateCallback(pa_stream* stream, void* userdata);
    CELL_FUNCTION_INTERNAL static void LatencyCallback(pa_stream* stream, void* userdata);

    Subsystem* subsystem;

    pa_stream* stream = nullptr;
    pa_stream_state_t state = PA_STREAM_UNCONNECTED;
    pa_usec_t latency = 0;

    size_t sampleSize;
};

}
