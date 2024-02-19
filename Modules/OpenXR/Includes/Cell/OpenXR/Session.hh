// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/OpenXR/Instance.hh>

namespace Cell::OpenXR {

class Session : public Object {
friend Instance;

public:
    // Terminates this session.
    CELL_FUNCTION ~Session();

    // TODO: store render target info and view structures in sessions, allow dispatching frames

    CELL_NON_COPYABLE(Session)

private:
    CELL_FUNCTION_INTERNAL CELL_INLINE Session(Instance* i, XrSystemId id, XrSystemProperties p, XrViewConfigurationView vc[2], XrSession s, XrSpace sp)
        : instance(i), id(id), properties(p), viewConfigurations(vc[0], vc[1]), session(s), space(sp) { }

    Instance* instance;

    XrSystemId id;
    XrSystemProperties properties;
    XrViewConfigurationView viewConfigurations[2];
    XrSession session;
    XrSpace space;
};

}
