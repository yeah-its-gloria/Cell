// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/DataManagement/Foreign/HTTP.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Entry.hh>

using namespace Cell;
using namespace Cell::DataManagement;
using namespace Cell::DataManagement::Foreign;

void CellEntry(Reference<System::String> parameterString) {
    (void)(parameterString);

    const System::String httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n<html><body>hi</body></html>";

    ScopedBlock<uint8_t> data = (uint8_t*)httpResponse.ToCharPointer();

    Wrapped<HTTP::Request*, Result> result = HTTP::Request::FromRaw(System::UnownedBlock<uint8_t> {data, httpResponse.GetSize()});
    CELL_ASSERT(result.IsValid());
}
