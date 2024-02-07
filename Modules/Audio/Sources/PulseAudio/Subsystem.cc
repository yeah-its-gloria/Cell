// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/Audio/Implementations/PulseAudio.hh>
#include <Cell/System/Panic.hh>

namespace Cell::Audio::Implementations::PulseAudio {

Wrapped<Subsystem*, Result> Subsystem::New(const System::String& title) {
    if (title.IsEmpty()) {
        return Result::InvalidParameters;
    }

    pa_threaded_mainloop* loop = pa_threaded_mainloop_new();
    CELL_ASSERT(loop != nullptr);

    ScopedBlock titleStr = title.ToCharPointer();
    pa_context* context = pa_context_new(pa_threaded_mainloop_get_api(loop), titleStr);
    CELL_ASSERT(context != nullptr);

    Subsystem* pulse = new Subsystem(loop, context);
    pa_context_set_state_callback(context, &Subsystem::StateCallback, (void*)pulse);

    int result = -pa_context_connect(context, nullptr, PA_CONTEXT_NOAUTOSPAWN, nullptr);
    switch (result) {
    case PA_OK: {
        break;
    }

    case PA_ERR_ACCESS: {
        pa_context_unref(context);
        pa_threaded_mainloop_free(loop);
        return Result::SubsystemBlocked;
    }

    case PA_ERR_CONNECTIONREFUSED:
    case PA_ERR_TIMEOUT:
    case PA_ERR_INVALIDSERVER:
    case PA_ERR_VERSION:
    case PA_ERR_NOTSUPPORTED: {
        pa_context_unref(context);
        pa_threaded_mainloop_free(loop);

        return Result::ServiceUnavailable;
    }

    default: {
        System::Panic("pa_context_connect failed");
    }
    }

    result = -pa_threaded_mainloop_start(loop);
    CELL_ASSERT(result == PA_OK);

    while (pulse->contextState != PA_CONTEXT_READY) {
        switch (pulse->contextState) {
        case PA_CONTEXT_UNCONNECTED:
        case PA_CONTEXT_CONNECTING:
        case PA_CONTEXT_AUTHORIZING:
        case PA_CONTEXT_SETTING_NAME: {
            pa_threaded_mainloop_wait(loop);
            break;
        }

        case PA_CONTEXT_READY: {
            break;
        }

        default: {
            result = -pa_context_errno(context);
            switch (result) {
            case PA_ERR_ACCESS: {
                pa_context_unref(context);
                pa_threaded_mainloop_stop(loop);
                pa_threaded_mainloop_free(loop);

                return Result::SubsystemBlocked;
            }

            case PA_ERR_CONNECTIONREFUSED:
            case PA_ERR_TIMEOUT:
            case PA_ERR_INVALIDSERVER:
            case PA_ERR_VERSION:
            case PA_ERR_NOTSUPPORTED: {
                pa_context_unref(context);
                pa_threaded_mainloop_stop(loop);
                pa_threaded_mainloop_free(loop);

                return Result::ServiceUnavailable;
            }

            default: {
                System::Panic("pa_context_connect failed during loop");
            }
            }
        }
        }
    }

    return pulse;
}

Subsystem::~Subsystem() {
    pa_threaded_mainloop_stop(this->loop);

    pa_context_disconnect(this->context);
    pa_context_unref(this->context);

    pa_threaded_mainloop_free(this->loop);
}

void Subsystem::StateCallback(pa_context* context, void* userdata) {
    Subsystem* pulse = (Subsystem*)userdata;
    CELL_ASSERT(pulse != nullptr && pulse->context == context);

    pulse->contextState = pa_context_get_state(pulse->context);
}

}
