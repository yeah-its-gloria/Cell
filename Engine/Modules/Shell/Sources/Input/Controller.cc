// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Shell/Input.hh>

#include <Cell/Shell/Controller/DualSense.hh>
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

    Wrapped<Controller::SwitchPro*, Result> switchProResult = Controller::SwitchPro::Find();
    if (switchProResult.IsValid()) {
        Controller::SwitchPro* switchPro = switchProResult.Unwrap();

        switchPro->SetProperties({
            .rumbleLeft = 0,
            .rumbleRight = 0,
            .playerIndex = 2
        });

        this->controllers.Append(switchPro);
    }

    Wrapped<Controller::Xbox*, Result> xboxResult = Controller::Xbox::Find();
    if (xboxResult.IsValid()) {
        Controller::Xbox* xbox = xboxResult.Unwrap();

        xbox->SetProperties({
            .rumbleLeft = 0,
            .rumbleRight = 0,
            .playerIndex = 3
        });

        this->controllers.Append(xbox);
    }

    return Result::Success;
}

}
