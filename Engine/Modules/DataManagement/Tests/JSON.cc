// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/DataManagement/Foreign/JSON.hh>
#include <Cell/IO/File.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Entry.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Panic.hh>

using namespace Cell;
using namespace Cell::DataManagement::Foreign;
using namespace Cell::System;

void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    ScopedObject<IO::File> file = IO::File::Open("./Engine/Modules/DataManagement/Content/Data.json").Unwrap();
    const size_t size = file->GetSize().Unwrap();

    OwnedBlock<char> data(size);
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
