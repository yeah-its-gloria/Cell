// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/DataManagement/JSON.hh>
#include <Cell/IO/File.hh>
#include <Cell/Memory/OwnedBlock.hh>
#include <Cell/System/Entry.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Panic.hh>

using namespace Cell;
using namespace Cell::DataManagement;
using namespace Cell::System;

void PrintValue(JSON::Value value, const bool isRoot = false, const bool isArray = false) {
    switch (value.type) {
    case JSON::Type::String: {
        ScopedBlock<char> string = value.string->ToCharPointer();
        Log("%: %", isArray ? "-" : value.name, &string);
        break;
    }

    case JSON::Type::Number: {
        Log("%: %", isArray ? "-" : value.name, value.number);
        break;
    }

    case JSON::Type::Boolean: {
        Log("%: %", isArray ? "-" : value.name, value.boolean ? "true" : "false");
        break;
    }

    case JSON::Type::Null: {
        Log("%: null", isArray ? "-" : value.name);
        break;
    }

    case JSON::Type::Object: {
        if (isRoot) {
            Log("(Document contains % top level element%)", value.count, value.count == 1 ? "" : "s");
        } else {
            Log("%: object, % element%", value.name, value.count, value.count == 1 ? "" : "s");
        }

        for (size_t i = 0; i < value.count; i++) {
            PrintValue(value.object[i]);
        }

        break;
    }

    case JSON::Type::Array: {
        Log("%: array, % elements", value.name, value.count);

        for (size_t i = 0; i < value.count; i++) {
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

    ScopedObject<IO::File> file = IO::File::Open("./Modules/DataManagement/Tests/Content/Data.json").Unwrap();
    const size_t size = file->GetSize();

    Memory::OwnedBlock<char> data(size);
    IO::Result result = file->Read(data);
    CELL_ASSERT(result == IO::Result::Success);

    String jsonData((char*)data.AsPointer(), size);
    ScopedObject document = JSON::Document::Parse(jsonData).Unwrap();

    PrintValue(document->GetRoot(), true);
}
