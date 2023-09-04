// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/System/Panic.hh>

CELL_FUNCTION extern "C" void __cxa_pure_virtual() {
    Cell::System::Panic("call to purely virtual function");
}
