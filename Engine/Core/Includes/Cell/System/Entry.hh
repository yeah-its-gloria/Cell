// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Reference.hh>
#include <Cell/System/String.hh>

namespace Cell::System {

// Entry function typedef used by the platform instantiation code.
typedef void (* EntryFunction)(Reference<String>);

}

// Title entry prototype. Needs to be defined by every title, utility or otherwise.
CELL_FUNCTION void CellEntry(Cell::Reference<Cell::System::String> parameterString);
