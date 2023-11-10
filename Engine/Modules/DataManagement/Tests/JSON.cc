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

void PrintValue(JSON::Value value, const bool isRoot = false, const bool isArray = false) {
    switch (value.type) {
    case JSON::Type::String: {
        ScopedBlock<char> string = value.string->ToCharPointer();
        Log("%s: %s", isArray ? "-" : value.name, &string);
        break;
    }

    case JSON::Type::Number: {
        Log("%s: %f", isArray ? "-" : value.name, value.number);
        break;
    }

    case JSON::Type::Boolean: {
        Log("%s: %s", isArray ? "-" : value.name, value.boolean ? "true" : "false");
        break;
    }

    case JSON::Type::Null: {
        Log("%s: null", isArray ? "-" : value.name);
        break;
    }

    case JSON::Type::Object: {
        if (isRoot) {
            Log("(Document contains %d top level elements)", value.size);
        } else {
            Log("%s: object, %d elements", value.name, value.size);
        }

        for (size_t i = 0; i < value.size; i++) {
            PrintValue(value.object[i]);
        }

        break;
    }

    case JSON::Type::Array: {
        Log("%s: array, %d elements", value.name, value.size);

        for (size_t i = 0; i < value.size; i++) {
            PrintValue(value.object[i], false, true);
        }

        break;
    }

    default: {
        CELL_UNIMPLEMENTED
    }
    }
}

void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    ScopedObject<IO::File> file = IO::File::Open("./Engine/Modules/DataManagement/Content/Data.json").Unwrap();
    const size_t size = file->GetSize().Unwrap();

    OwnedBlock<char> data(size);
    IO::Result result = file->Read(data);
    CELL_ASSERT(result == IO::Result::Success);

    String jsonData(data);
    ScopedObject document = JSON::Document::Parse(jsonData).Unwrap();

    PrintValue(document->GetRoot(), true);
}
