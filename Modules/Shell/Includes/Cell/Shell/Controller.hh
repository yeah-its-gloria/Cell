// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Shell/InputTypes.hh>
#include <Cell/Shell/Result.hh>

namespace Cell::Shell {

struct ControllerReport {
    ControllerButton buttons;

    double leftStickX;
    double leftStickY;

    double rightStickX;
    double rightStickY;

    double leftTrigger;
    double rightTrigger;
};

struct ControllerProperties {
    uint8_t rumbleLeft;
    uint8_t rumbleRight;

    uint8_t playerIndex;
};

class IController : public Object {
public:
    // Destroys the controller instance.
    virtual ~IController() = default;

    // Updates all data concerning the controller.
    virtual Result Update() = 0;

    // Returns the currently reported controller data.
    CELL_INLINE ControllerReport GetReport() {
        return this->report;
    }

    // Returns the report stored prior to an Update call.
    CELL_INLINE ControllerReport GetPreviousReport() {
        return this->lastReport;
    }

    CELL_INLINE void SetProperties(ControllerProperties properties) {
        this->properties = properties;
        this->hasUpdated = false;
    }

protected:
    ControllerReport report = { };
    ControllerReport lastReport = { };
    ControllerProperties properties = { };
    bool hasUpdated = false;
};

}
