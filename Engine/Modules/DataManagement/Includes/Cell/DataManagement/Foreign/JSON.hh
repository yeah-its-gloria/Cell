// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/List.hh>
#include <Cell/DataManagement/Result.hh>
#include <Cell/System/String.hh>

namespace Cell::DataManagement::Foreign::JSON {

enum class Type {
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
        Value* array;  // values are unnamed
        System::String* string; // UTF-8, not null terminated!
        double number;
        bool boolean;
    };

    size_t size; // length for string, 0 for num/bool/null, count for object/array
};

// Parses a complete JSON document.
CELL_FUNCTION Wrapped<List<Value>, Result> Parse(const System::String& document);

// Frees a JSON list.
CELL_FUNCTION void Free(List<Value>& data);

}
