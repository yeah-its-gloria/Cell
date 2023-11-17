// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Entry.hh>
#include <Cell/Mathematics/Vector2.hh>
#include <Cell/Mathematics/Vector3.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Panic.hh>

using namespace Cell;

void CellEntry(Reference<System::String> parameterString) {
    (void)(parameterString);

    // TODO: better testing

    System::Log("Testing: magnitude");

    Mathematics::Vector2 vector2(2.f, 2.f);
    CELL_ASSERT(vector2.Magnitude() == 2.82842708f);

    Mathematics::Vector3 vector3(2.f, 2.f, 2.f);
    CELL_ASSERT(vector3.Magnitude() == 3.46410155f);
}
