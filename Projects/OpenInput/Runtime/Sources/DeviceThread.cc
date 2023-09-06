// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Devices/DualSense.hh"
#include "Devices/GameCubeAdapter.hh"
#include "Devices/Wiimote.hh"
#include "Devices/Wiimote/Communications/ClassicController.hh"
#include "Devices/Wiimote/Communications/Nunchuck.hh"

#include "Server.hh"

#include <Cell/System/Thread.hh>

using namespace Cell;

namespace Runtime {

using namespace Devices;
using namespace Devices::WiimoteCommunications;

void Server::DeviceThread() {
    const Wrapped<GameCubeAdapter*, Result> gccResult = GameCubeAdapter::Find();
    if (!gccResult) {
        System::Log("No GameCube Controller found");
    } else {
        this->deviceCount++;
    }

    const Wrapped<DualSense*, Result> dsResult = DualSense::Find();
    if (!dsResult) {
        System::Log("No DualSense Controller found");
    } else {
        this->deviceCount++;
    }

    const Wrapped<Wiimote*, Result> wiiResult = Wiimote::Find();
    if (!wiiResult) {
        System::Log("No Wiimote Controller found");
    } else {
        this->deviceCount++;
    }

    GameCubeAdapter* gameCube = gccResult.UnwrapOr(nullptr);
    DualSense* dualSense = dsResult.UnwrapOr(nullptr);
    Wiimote* wiimote = wiiResult.UnwrapOr(nullptr);

    if (wiimote != nullptr) {
        const Result result = wiimote->SetLEDs(LEDMask::LED1);
        CELL_ASSERT(result == Devices::Result::Success);
    }

    while (this->isActive) {
        if (gameCube != nullptr) {
            const Result result = gameCube->Poll();
            CELL_ASSERT(result == Result::Success);
        }

        if (dualSense != nullptr) {
            const Result result = dualSense->Poll();
            CELL_ASSERT(result == Result::Success);
        }

        if (wiimote != nullptr) {
            const Result result = wiimote->Poll();
            if (result == Result::Success) {
                const ButtonsAccelExtensionReport inputData = wiimote->GetInputData();
                switch (wiimote->GetExtensionType()) {
                case WiimoteExtensionType::None:
                case WiimoteExtensionType::Unknown: {
                    System::Log("B: %s", (uint16_t)(Buttons::B & inputData.buttons) ? "X" : "-");
                    break;
                }

                case WiimoteExtensionType::Nunchuck: {
                    const NunchuckReport* nunchuck = (NunchuckReport*)inputData.extensionData;
                    System::Log("A: %s Nunchuck X: %d Y: %d C: %s",
                                (uint16_t)(Buttons::A & inputData.buttons) ? "X" : "-",
                                nunchuck->stickX, nunchuck->stickY,
                                (uint8_t)(NunchuckButtons::C & (nunchuck->buttons ^ NunchuckButtons::Mask)) ? "X" : "-");

                    break;
                }

                case WiimoteExtensionType::ClassicController:
                case WiimoteExtensionType::ClassicControllerPro: {
                    const ClassicControllerReport* classicController = (ClassicControllerReport*)inputData.extensionData;
                    System::Log("CC X: %d Y: %d a: %s",
                                classicController->leftStickX, classicController->leftStickY,
                                (uint16_t)(ClassicControllerButtons::a & (classicController->buttons ^ ClassicControllerButtons::Mask)) ? "X" : "-");

                    break;
                }
                }
            }

            CELL_ASSERT(result == Result::Timeout);
        }

        System::Thread::Yield();
    }

    if (wiimote != nullptr) {
        delete wiimote;
    }

    if (dualSense != nullptr) {
        delete dualSense;
    }

    if (gameCube != nullptr) {
        delete gameCube;
    }
}

}
