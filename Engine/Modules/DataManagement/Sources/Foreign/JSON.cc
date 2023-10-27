// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/DataManagement/Foreign/JSON.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Panic.hh>
#include <Cell/Utilities/RawString.hh>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

namespace Cell::DataManagement::Foreign::JSON {

enum class seekResult {
    Success,
    FoundDifferentCharacter,
    ReachedEOF,
};

seekResult seekNextToken(size_t& position, const char token, const char* document, size_t starting_offset = 0, bool ignoreJustEmptySpace = true) {
    size_t offset = starting_offset;
    for (; offset < strlen(document); offset++) {
        if (document[offset] == token) {
            break;
        }

        if (document[offset] == ' ' || document[offset] == '\n') {
            continue;
        }

        if (ignoreJustEmptySpace) {
            return seekResult::FoundDifferentCharacter;
        }
    }

    if (offset == strlen(document) && document[offset] != token) {
        return seekResult::ReachedEOF;
    }

    position = offset;
    return seekResult::Success;
}

Wrapped<Collection::List<Value>, Result> Parse(const System::String& string) {
    if (string.IsEmpty()) {
        return Result::InvalidParameters;
    }

    const char* document = string.ToRawPointer();

    Collection::List<Value> values;

    // seek to beginning
    size_t position = 0;
    seekResult seek_result = seekNextToken(position, '{', document);
    CELL_ASSERT(seek_result == seekResult::Success);

    position++;

    Value value {};
    while (true) {
        // find the beginning of the key
        seek_result = seekNextToken(position, '"', document, position);
        CELL_ASSERT(seek_result == seekResult::Success);

        // find its end
        size_t end = 0;
        seek_result = seekNextToken(end, '"', document, position + 1, false);
        CELL_ASSERT(seek_result == seekResult::Success);

        // allocate it
        value.name = System::AllocateMemory<char>(end - position + 1);
        System::CopyMemory<char>(value.name, document + position + 1, end - position - 1);

        // move on
        position = end + 1;
        CELL_ASSERT(document[position++] == ':');

        // ignore empty space or newline
        while (document[position] == ' ' || document[position] == '\n') {
            position++;
        }

        // check the value
        switch (document[position]) {
        // string
        case '"': {
            size_t string_end = 0;
            seek_result = seekNextToken(string_end, '"', document, position + 1, false);
            CELL_ASSERT(seek_result == seekResult::Success);

            value.type = Type::String;
            value.size = string_end - position - 1;
            value.string = new System::String(document + position + 1, value.size);

            position = string_end + 1;
            break;
        }

        // true boolean
        case 't': {
            CELL_ASSERT(strncmp(document + position, "true", 4) == 0);

            value.type = Type::Boolean;
            value.size = 0;
            value.boolean = true;

            position += Utilities::RawStringSize("true");
            break;
        }

        // false boolean
        case 'f': {
            CELL_ASSERT(strncmp(document + position, "false", 5) == 0);

            value.type = Type::Boolean;
            value.size = 0;
            value.boolean = false;

            position += Utilities::RawStringSize("false");
            break;
        }

        // null
        case 'n': {
            CELL_ASSERT(strncmp(document + position, "null", 4) == 0);

            value.type = Type::Null;
            value.size = 0;
            value.object = nullptr;

            position += Utilities::RawStringSize("null");
            break;
        }

        // array
        case '[': {
            System::Panic("ow ow");
        }

        // object
        case '{': {
            System::Panic("ouch");
        }

        // assume number, fail if not
        default: {
            size_t numberEnd = 0;
            seek_result = seekNextToken(numberEnd, ',', document, position + 1, false);
            switch (seek_result) {
            case seekResult::Success: {
                break;
            }

            case seekResult::ReachedEOF: {
                seek_result = seekNextToken(numberEnd, ' ', document, position + 1, false); // this fails if there's no empty space or newline
                CELL_ASSERT(seek_result == seekResult::Success);
                break;
            }

            default: {
                CELL_UNIMPLEMENTED
            }
            }

            System::OwnedBlock<char> numberString(numberEnd - position + 1);
            System::CopyMemory(numberString, document + position, numberEnd - position);

            value.type = Type::Number;
            value.size = 0;
            value.number = strtod(numberString, nullptr);

            CELL_ASSERT(errno != ERANGE);

            position = numberEnd + 1;
            break;
        }
        }

        values.Append(value);

        CELL_ASSERT(document[position] == ',' || document[position] == ' ' || document[position] == '\n');
        position++;

        if (document[position] == '}') {
            break;
        }

        while (document[position] == ' ' || document[position] == '\n') {
            position++;
        }
    }

    return values;
}

CELL_INLINE void Free(Value* child) {
    if ((child->type == Type::Object || child->type == Type::Array) && child->size > 0) {
        for (size_t i = 0; i < child->size; i++) {
            Free(child->object);
        }
    }

    switch (child->type) {
    case Type::Object: {
        System::FreeMemory(child->name);
        System::FreeMemory(child->object);
        break;
    }

    case Type::Array: {
        System::FreeMemory(child->array);
        break;
    }

    case Type::String: {
        delete child->string;
        break;
    }

    case Type::Number:
    case Type::Boolean:
    case Type::Null: {
        break;
    }

    default: {
        CELL_UNREACHABLE;
    }
    }
}

void Free(Collection::List<Value>& data) {
    for (Value value : data) {
        Free(&value);
    }
}

}
