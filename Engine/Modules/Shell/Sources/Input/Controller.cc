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

        dualSense->UpdateProperties({
            .ledData = 0x0000feff,
            .rumbleLeft = 0, .rumbleRight = 0,
            .playerLedOn = true
        });

        this->controllers.Append(dualSense);
    }

    Wrapped<Controller::SwitchPro*, Result> switchResult = Controller::SwitchPro::Find();
    if (switchResult.IsValid()) {
        this->controllers.Append(switchResult.Unwrap());
    }

    Wrapped<Controller::Xbox*, Result> xboxResult = Controller::Xbox::Find();
    if (xboxResult.IsValid()) {
        this->controllers.Append(xboxResult.Unwrap());
    }

    return Result::Success;
}

}
