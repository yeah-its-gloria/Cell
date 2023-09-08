// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/DataManagement/Foreign/glTF.hh>
#include <Cell/DataManagement/Foreign/HTTP.hh>
#include <Cell/DataManagement/Foreign/JSON.hh>
#include <Cell/DataManagement/Foreign/PNG.hh>
#include <Cell/IO/File.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Entry.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Panic.hh>

using namespace Cell;
using namespace Cell::DataManagement::Foreign;
using namespace Cell::System;

CELL_FUNCTION_INTERNAL void TestJSON() {
    ScopedObject<IO::File> file = IO::File::Open("./Engine/Modules/DataManagement/Content/Data.json").Unwrap();
    const size_t size = file->GetSize().Unwrap();

    ManagedBlock<char> data(size);
    IO::Result result = file->Read(data);
    CELL_ASSERT(result == IO::Result::Success);

    String jsonData(data);
    List<JSON::Value> values = JSON::Parse(jsonData).Unwrap();

    for (JSON::Value value : values) {
        switch (value.type) {
        case JSON::Type::String: {
            ScopedBlock<char> string = value.string->ToCharPointer();
            Log("%s: %s", value.name, &string);
            break;
        }

        case JSON::Type::Number: {
            Log("%s: %f", value.name, value.number);
            break;
        }

        case JSON::Type::Boolean: {
            Log("%s: %s", value.name, value.boolean ? "true" : "false");
            break;
        }

        case JSON::Type::Null: {
            Log("%s: null", value.name);
            break;
        }

        default: {
            CELL_UNIMPLEMENTED
        }
        }
    }

    JSON::Free(values);
}

CELL_FUNCTION_INTERNAL void TestglTF() {
    ScopedObject<IO::File> file = IO::File::Open("./Engine/Modules/DataManagement/Content/Box.glb").Unwrap();
    const size_t size = file->GetSize().Unwrap();

    ManagedBlock<uint8_t> data(size);
    IO::Result result = file->Read(data);
    CELL_ASSERT(result == IO::Result::Success);

    ParseGlTF(data, size);
}

CELL_FUNCTION_INTERNAL void TestPNG() {
    ScopedObject<IO::File> file = IO::File::Open("./Engine/Modules/DataManagement/Content/Trans.png").Unwrap();
    const size_t size = file->GetSize().Unwrap();

    ManagedBlock<uint8_t> data(size);
    IO::Result result = file->Read(data);
    CELL_ASSERT(result == IO::Result::Success);

    PNG* png = PNG::Decode(data, size).Unwrap();
    delete png;
}

CELL_FUNCTION_INTERNAL void TestHTTP() {
    const String httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n<html><body>hi</body></html>";

    ScopedBlock<uint8_t> data = (uint8_t*)httpResponse.ToCharPointer();
    HTTPParseResponse(&data, httpResponse.GetLength());
}

void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    //Log("Testing JSON");
    //TestJSON();

    //Log("Testing glTF");
    //TestglTF();

    //Log("Testing PNG");
    //TestPNG();

    Log("Testing HTTP");
    TestHTTP();
}
