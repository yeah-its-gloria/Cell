// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include "../Tools.hh"

#include <Cell/Shell/Input.hh>

using namespace Cell;
using namespace Cell::Shell;

const float MovementSpeed = 5.f;

void VulkanToolsInputSetUp(Input* input, InputData* data) {
    Result shellResult = input->RegisterAction(
        KeyValue::W, [](const KeyValue value, void* data) {
            (void)(value);

            InputData* inputData = (InputData*)data;
            inputData->position->y += MovementSpeed * inputData->instance->GetDelta();
        },
        data);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(
        KeyValue::S, [](const KeyValue value, void* data) {
            (void)(value);

            InputData* inputData = (InputData*)data;
            inputData->position->y -= MovementSpeed * inputData->instance->GetDelta();
        },
        data);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(
        KeyValue::A, [](const KeyValue value, void* data) {
            (void)(value);

            InputData* inputData = (InputData*)data;
            inputData->position->x += MovementSpeed * inputData->instance->GetDelta();
        },
        data);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(
        KeyValue::D, [](const KeyValue value, void* data) {
            (void)(value);

            InputData* inputData = (InputData*)data;
            inputData->position->x -= MovementSpeed * inputData->instance->GetDelta();
        },
        data);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(
        KeyValue::Q, [](const KeyValue value, void* data) {
            (void)(value);

            InputData* inputData = (InputData*)data;
            inputData->position->z -= MovementSpeed * inputData->instance->GetDelta();
        },
        data);
    CELL_ASSERT(shellResult == Result::Success);

    shellResult = input->RegisterAction(
        KeyValue::E, [](const KeyValue value, void* data) {
            (void)(value);

            InputData* inputData = (InputData*)data;
            inputData->position->z += MovementSpeed * inputData->instance->GetDelta();
        },
        data);
    CELL_ASSERT(shellResult == Result::Success);
}
