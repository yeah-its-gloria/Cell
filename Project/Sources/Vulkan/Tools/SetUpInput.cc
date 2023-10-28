// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Tools.hh"
#include <Cell/Shell/Input.hh>

using namespace Cell;
using namespace Cell::Shell;

const float MovementSpeed = 5.f;

void VulkanToolsInputSetUp(Input* input, InputData* data) {
    Result shellResult = input->RegisterAction(
        KeyboardButton::W, [](const KeyboardButton value, void* data) {
            (void)(value);

            InputData* inputData = (InputData*)data;
            inputData->position->y += MovementSpeed * inputData->instance->GetDelta();
        },
        data);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(
        KeyboardButton::S, [](const KeyboardButton value, void* data) {
            (void)(value);

            InputData* inputData = (InputData*)data;
            inputData->position->y -= MovementSpeed * inputData->instance->GetDelta();
        },
        data);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(
        KeyboardButton::A, [](const KeyboardButton value, void* data) {
            (void)(value);

            InputData* inputData = (InputData*)data;
            inputData->position->x += MovementSpeed * inputData->instance->GetDelta();
        },
        data);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(
        KeyboardButton::D, [](const KeyboardButton value, void* data) {
            (void)(value);

            InputData* inputData = (InputData*)data;
            inputData->position->x -= MovementSpeed * inputData->instance->GetDelta();
        },
        data);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(
        KeyboardButton::Q, [](const KeyboardButton value, void* data) {
            (void)(value);

            InputData* inputData = (InputData*)data;
            inputData->position->z -= MovementSpeed * inputData->instance->GetDelta();
        },
        data);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(
        KeyboardButton::E, [](const KeyboardButton value, void* data) {
            (void)(value);

            InputData* inputData = (InputData*)data;
            inputData->position->z += MovementSpeed * inputData->instance->GetDelta();
        },
        data);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(
        KeyboardButton::F, [](const KeyboardButton value, void* data) {
            (void)(value);

            InputData* inputData = (InputData*)data;
            inputData->instance->Trigger();
        },
        data);
    CELL_ASSERT(shellResult == Result::Success);
}
