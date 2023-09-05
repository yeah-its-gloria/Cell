// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "Devices/DualSense.hh"
#include "Devices/GameCubeAdapter.hh"
#include "Devices/Wiimote.hh"
#include "Devices/Wiimote/Communications/ClassicController.hh"
#include "Devices/Wiimote/Communications/Nunchuck.hh"

#include "Server.hh"

#include <Cell/Scoped.hh>
#include <Cell/System/Thread.hh>

using namespace Cell;

namespace Runtime {

using namespace Devices;
using namespace Devices::WiimoteCommunications;

void Server::DeviceThread() {
    const Wrapped<GameCubeAdapter*, Result> gccResult = GameCubeAdapter::Find();
    if (!gccResult) {
        System::Panic("Runtime::Devices::GameCubeAdapter::Find failed");
    }

    ScopedObject<GameCubeAdapter> gcc = gccResult.Unwrap();

    /*DualSense::Find();

    const Wrapped<Wiimote*, Result> wiimoteResult = Wiimote::Find();
    if (!wiimoteResult) {
        switch (wiimoteResult.Result()) {
        case Result::InvalidData: {
            System::Log("Wiimote is in an invalid state for setup, please reset");
            return;
        }

        case Result::NotFound: {
            System::Log("Wiimote not found");
            return;
        }

        default: {
            System::Panic("Wiimote::Find failed");
        }
        }
    }

    ScopedObject<Wiimote> wiimote = wiimoteResult.Unwrap();

    Result result = wiimote->SetLEDs(LEDMask::LED1);
    CELL_ASSERT(result == Devices::Result::Success);*/

    while (true) {
        /*result = wiimote->Poll();
        if (result == Result::Timeout) {
            System::Thread::Yield();
            continue;
        }

        CELL_ASSERT(result == Result::Success);

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
        }*/

        gcc->Poll();

        System::Thread::Yield();
    }
}

}
