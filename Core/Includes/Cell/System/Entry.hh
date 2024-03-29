// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Reference.hh>
#include <Cell/String.hh>

namespace Cell::System {

// Entry function typedef used by the platform instantiation code.
typedef void (* EntryFunction)(Reference<String>);

}

// Title entry prototype. Needs to be defined by every title, test and utility.
CELL_FUNCTION_INTERNAL void CellEntry(Cell::Reference<Cell::String> parameterString);
