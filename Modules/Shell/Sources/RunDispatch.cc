// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Shell.hh>
#include <Cell/System/Log.hh>

#include <math.h>

namespace Cell::Shell {

#define CONTROLLER_AXIS_CASE(axisName, reportName) case ControllerAxis::axisName: { if (fabs(report.reportName) > 0.1) { info.axis(report.reportName, info.userData); } break; }

template <typename B> void handleButton(B current, B previous, B match, ButtonFunction function, void* userData) {
    switch ((((current & match) == match) ? 1 : 0) | ((previous & match) == match ? 2 : 0)) {
    case  1: { function(InputType::Pressed,  userData); break; } // current
    case  2: { function(InputType::Released, userData); break; } // previous
    case  3: { function(InputType::Held,     userData); break; } // both
    default: {                                          break; } // none
    }
}

Result IShell::RunDispatch() {
    Result result = this->RunDispatchImpl();
    if (result != Result::Success) {
        return result;
    }

    if (!this->IsInForeground()) {
        return Result::Success;
    }

    Collection::List<ControllerReport> reports;
    Collection::List<ControllerReport> previousReports;

    if (this->controllers.GetCount() > 0) {
        for (IController* controller : this->controllers) {
            Result result = controller->Update();
            switch (result) {
            case Result::Success: {
                reports.Append(controller->GetReport());
                previousReports.Append(controller->GetPreviousReport());
                break;
            }

            case Result::Timeout: {
                System::Log("Controller failed to respond!");
                break;
            }

            default: {
                System::Panic("Cell::Shell::IController::Update failed");
            }
            }
        }
    }

    for (RegisterInfo info : this->registeredFunctions) {
        switch (info.type) {
        case 0: { // keyboard button
            handleButton(this->keys, this->oldKeys, info.keyboard, info.button, info.userData);
            break;
        }

        case 1: { // mouse button
            break;
        }

        case 2: { // controller button
            for (uint32_t index = 0; index < reports.GetCount(); index++) {
                handleButton(reports[index].buttons, previousReports[index].buttons, info.controller, info.button, info.userData);
            }

            break;
        }

        case 3: { // mouse axis
            break;
        }

        case 4: { // controller axis
            for (ControllerReport report : reports) {
                switch (info.controllerAxis) {
                CONTROLLER_AXIS_CASE(LeftStickX, leftStickX)
                CONTROLLER_AXIS_CASE(LeftStickY, leftStickY)
                CONTROLLER_AXIS_CASE(RightStickX, rightStickX)
                CONTROLLER_AXIS_CASE(RightStickY, rightStickY)
                CONTROLLER_AXIS_CASE(LeftTrigger, leftTrigger)
                CONTROLLER_AXIS_CASE(RightTrigger, rightTrigger)

                // TODO: touchpad data

                default: { break; }
                }
            }
        }
        }
    }

    this->oldKeys = this->keys;
    return Result::Success;
}

}
