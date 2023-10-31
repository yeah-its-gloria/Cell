// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Implementations/Windows.hh>
#include <Cell/Shell/Input.hh>
#include <Cell/System/Log.hh>

#include <math.h>

namespace Cell::Shell {
using namespace Implementations;

Result Input::Poll() {
    if (!this->shell.IsInForeground()) {
        return Result::Success;
    }

    Collection::List<ControllerReport> reports;
    Collection::List<ControllerReport> previousReports;

    if (this->controllers.GetCount() > 0) {
        for (IController* controller : this->controllers) {
            Result result = controller->Update();
            if (result != Result::Success) {
                return result;
            }

            reports.Append(controller->GetReport());
            previousReports.Append(controller->GetPreviousReport());
        }
    }

    KeyboardButton keys = this->shell.GetKeys();
    KeyboardButton oldKeys = this->shell.GetPreviousKeys();

    for (RegisterInfo info : this->registeredFunctions) {
        switch (info.type) {
        case 0: { // keyboard button
            if ((oldKeys & info.keyboard) != info.keyboard && (keys & info.keyboard) == info.keyboard) {
                info.button(InputType::Pressed, info.userData);
            }

            if ((oldKeys & info.keyboard) == info.keyboard && (keys & info.keyboard) != info.keyboard) {
                info.button(InputType::Released, info.userData);
            }

            if ((oldKeys & info.keyboard) == info.keyboard && (keys & info.keyboard) == info.keyboard) {
                info.button(InputType::Held, info.userData);
            }

            break;
        }

        case 1: { // mouse button
            break;
        }

        case 2: { // controller button
            for (uint32_t index = 0; index < reports.GetCount(); index++) {
                if ((previousReports[index].buttons & info.controller) != info.controller && (reports[index].buttons & info.controller) == info.controller) {
                    info.button(InputType::Pressed, info.userData);
                }

                if ((previousReports[index].buttons & info.controller) == info.controller && (reports[index].buttons & info.controller) != info.controller) {
                    info.button(InputType::Released, info.userData);
                }

                if ((previousReports[index].buttons & info.controller) == info.controller && (reports[index].buttons & info.controller) == info.controller) {
                    info.button(InputType::Held, info.userData);
                }
            }

            break;
        }

        case 3: { // mouse axis
            break;
        }

        case 4: { // controller axis
            for (ControllerReport report : reports) {
                switch (info.controllerAxis) {
                case ControllerAxis::LeftStickX: {
                    if (fabs(report.leftStickX) > 0.1) {
                        info.axis(report.leftStickX, info.userData);
                    }

                    break;
                }

                case ControllerAxis::LeftStickY: {
                    if (fabs(report.leftStickY) > 0.1) {
                        info.axis(report.leftStickY, info.userData);
                    }

                    break;
                }

                case ControllerAxis::RightStickX: {
                    if (fabs(report.rightStickX) > 0.1) {
                        info.axis(report.rightStickX, info.userData);
                    }

                    break;
                }

                case ControllerAxis::RightStickY: {
                    if (fabs(report.rightStickY) > 0.1) {
                        info.axis(report.rightStickY, info.userData);
                    }

                    break;
                }

                case ControllerAxis::LeftTrigger: {
                    if (fabs(report.leftTrigger) > 0.1) {
                        info.axis(report.leftTrigger, info.userData);
                    }

                    break;
                }

                case ControllerAxis::RightTrigger: {
                    if (fabs(report.rightTrigger) > 0.1) {
                        info.axis(report.rightTrigger, info.userData);
                    }

                    break;
                }

                default: { // TODO: touchpad data
                    break;
                }
                }
            }
        }
        }
    }

    this->shell.MoveKeysToOld();
    return Result::Success;
}

}
