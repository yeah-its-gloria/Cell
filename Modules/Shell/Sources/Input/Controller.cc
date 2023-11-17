// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Input.hh>

#include <Cell/Shell/Controller/DualSense.hh>
#include <Cell/Shell/Controller/DualShock4.hh>
#include <Cell/Shell/Controller/SwitchPro.hh>
#include <Cell/Shell/Controller/Xbox.hh>

namespace Cell::Shell {

Result Input::DiscoverControllers() {
    Wrapped<Controller::DualSense*, Result> dualSenseResult = Controller::DualSense::Find();
    if (dualSenseResult.IsValid()) {
        Controller::DualSense* dualSense = dualSenseResult.Unwrap();

        dualSense->SetProperties({
            .rumbleLeft = 0,
            .rumbleRight = 0,
            .playerIndex = 1
        });

        this->controllers.Append(dualSense);
    }

    Wrapped<Controller::DualShock4*, Result> dualShock4Result = Controller::DualShock4::Find();
    if (dualShock4Result.IsValid()) {
        Controller::DualShock4* dualShock4 = dualShock4Result.Unwrap();

        dualShock4->SetProperties({
            .rumbleLeft = 0,
            .rumbleRight = 0,
            .playerIndex = 2
        });

        this->controllers.Append(dualShock4);
    }

    Wrapped<Controller::SwitchPro*, Result> switchProResult = Controller::SwitchPro::Find();
    if (switchProResult.IsValid()) {
        Controller::SwitchPro* switchPro = switchProResult.Unwrap();

        switchPro->SetProperties({
            .rumbleLeft = 0,
            .rumbleRight = 0,
            .playerIndex = 3
        });

        this->controllers.Append(switchPro);
    }

    Wrapped<Controller::Xbox*, Result> xboxResult = Controller::Xbox::Find();
    if (xboxResult.IsValid()) {
        Controller::Xbox* xbox = xboxResult.Unwrap();

        xbox->SetProperties({
            .rumbleLeft = 0,
            .rumbleRight = 0,
            .playerIndex = 4
        });

        this->controllers.Append(xbox);
    }

    return Result::Success;
}

}
