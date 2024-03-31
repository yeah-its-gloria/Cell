// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/DataManagement/HTTP.hh>
#include <Cell/Memory/UnownedBlock.hh>
#include <Cell/System/Entry.hh>

using namespace Cell;
using namespace Cell::DataManagement;

void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    const String httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n<html><body>hi</body></html>";

    ScopedBlock<uint8_t> data = (uint8_t*)httpResponse.ToCharPointer();

    Wrapped<HTTP::Request*, Result> result = HTTP::Request::FromRaw(Memory::UnownedBlock<uint8_t> { data, httpResponse.GetSize() });
    CELL_ASSERT(result.IsValid());
}
