// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Collection/List.hh>
#include <Cell/DataManagement/Result.hh>
#include <Cell/String.hh>

namespace Cell::DataManagement::JSON {

enum class Type : uint8_t {
    Object,
    Array,
    String,
    Number,
    Boolean,
    Null
};

struct Value {
    char* name;
    Type type;

    union {
        Value* object;
        Value* array; // values are unnamed
        String* string; // UTF-8
        double number;
        bool boolean;
    };

    size_t size; // length for string, 0 for num/bool/null, count for object/array
};

class Document : public Object {
public:
    CELL_FUNCTION static Wrapped<Document*, Result> Parse(const String& document);
    CELL_FUNCTION ~Document();

    CELL_FUNCTION Value GetRoot();

private:
    CELL_FUNCTION_INTERNAL Document(Value r) : root(r) { }

    Value root;
};

}
