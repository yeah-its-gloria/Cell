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
    // Name of the value. This is empty for values within array type values.
    String name;

    // Indicates the type of value stored.
    Type type;

    union {
        Value* object;
        Value* array;
        String* string;
        double number;
        bool boolean;
    };

    // Counts the number of values that are part of this value.
    // Applies only to object and array type values.
    size_t count;
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
