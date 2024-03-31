// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/OpenXR/Instance.hh>

namespace Cell::OpenXR {

class Session : public NoCopyObject {
friend Instance;

public:
    // Terminates this session.
    CELL_FUNCTION ~Session();

    // TODO: store render target info and view structures in sessions, allow dispatching frames

private:
    CELL_FUNCTION_INTERNAL Session(Instance* i, XrSystemId id, XrSystemProperties p, XrViewConfigurationView vc[2], XrSession s, XrSpace sp)
        : instance(i), id(id), properties(p), viewConfigurations { vc[0], vc[1] }, session(s), space(sp) { }

    Instance* instance;

    XrSystemId id;
    XrSystemProperties properties;
    XrViewConfigurationView viewConfigurations[2];
    XrSession session;
    XrSpace space;
};

}
