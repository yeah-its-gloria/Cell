// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/DataManagement/Foreign/JSON.hh>
#include <Cell/System/BlockImpl.hh>
#include <Cell/System/Log.hh>

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

CELL_FUNCTION_INTERNAL size_t parseObject(Collection::List<Value>& values, const char* document, const size_t size, uint8_t& recursionCounter);

CELL_FUNCTION_INTERNAL size_t parseValue(Value& value, const char* document, const size_t size, uint8_t& recursionCounter) {
    // TODO: instead of panicking, we should inform the caller that this JSON is kinda garbage
    CELL_ASSERT(recursionCounter < 25);

    recursionCounter++;

    size_t position = 0;
    seekResult seekResult = seekResult::Success;

    switch (document[position]) {
    case '"': { // string
        size_t string_end = 0;
        seekResult = seekNextToken(string_end, '"', document, position + 1, false);
        CELL_ASSERT(seekResult == seekResult::Success);

        value.type = Type::String;
        value.size = string_end - position - 1;
        value.string = new String(document + position + 1, value.size);

        position = string_end + 1;
        break;
    }

    case 't': { // true boolean
        CELL_ASSERT(strncmp(document + position, "true", 4) == 0);

        value.type = Type::Boolean;
        value.size = 0;
        value.boolean = true;

        position += StringDetails::RawStringSize("true");
        break;
    }

    case 'f': { // false boolean
        CELL_ASSERT(strncmp(document + position, "false", 5) == 0);

        value.type = Type::Boolean;
        value.size = 0;
        value.boolean = false;

        position += StringDetails::RawStringSize("false");
        break;
    }

    case 'n': { // null
        CELL_ASSERT(strncmp(document + position, "null", 4) == 0);

        value.type = Type::Null;
        value.size = 0;
        value.object = nullptr;

        position += StringDetails::RawStringSize("null");
        break;
    }

    case '[': { // array
        while (document[position] != ' ') { // skip past the opening bracket and anything like a newline
            position++;
        }

        size_t endPosition = position;
        while (document[endPosition] != ']') {
            CELL_ASSERT(document[endPosition] != '['); // TODO: allow sub arrays
            endPosition++;
        }

        position += endPosition - position;

        //Collection::List<Value> elements;

        value.type = Type::Array;
        value.size = 0; //elements.GetCount();
        value.array = nullptr;

        /*value.array = System::AllocateMemory<Value>(elements.GetCount());
        for (size_t i = 0; i < elements.GetCount(); i++) {
            value.array[i] = elements[i];
        }*/

        position++;
        break;
    }

    case '{': { // object
        while (document[position] != ' ') { // skip past the opening brace and anything like a newline
            position++;
        }

        Collection::List<Value> elements;
        size_t endPosition = parseObject(elements, document + position, size - position, recursionCounter);

        value.type = Type::Object;
        value.size = elements.GetCount();

        value.object = System::AllocateMemory<Value>(elements.GetCount());
        for (size_t i = 0; i < elements.GetCount(); i++) {
            value.object[i] = elements[i];
        }

        position += endPosition + 1;
        break;
    }

    default: { // assume number, fail if not
        size_t numberEnd = 0;
        seekResult = seekNextToken(numberEnd, ',', document, position + 1, false);
        switch (seekResult) {
        case seekResult::Success: {
            break;
        }

        case seekResult::ReachedEOF: {
            seekResult = seekNextToken(numberEnd, ' ', document, position + 1, false); // this fails if there's no empty space or newline
            CELL_ASSERT(seekResult == seekResult::Success);
            break;
        }

        default: {
            CELL_UNREACHABLE;
        }
        }

        System::OwnedBlock<char> numberString(numberEnd - position + 1);
        System::CopyMemory(numberString, document + position, numberEnd - position);

        value.type = Type::Number;
        value.size = 0;
        value.number = strtod(numberString, nullptr);

        position = numberEnd + 1;
        break;
    }
    }

    recursionCounter--;

    return position;
}

// Assumes position is past opening brace
size_t parseObject(Collection::List<Value>& values, const char* document, const size_t size, uint8_t& recursionCounter) {
    // TODO: instead of panicking, we should inform the caller that this JSON is kinda garbage
    CELL_ASSERT(recursionCounter < 25);

    recursionCounter++;

    size_t position = 0;
    while (position < size) {
        Value value;

        // find the beginning of the key
        seekResult seekResult = seekNextToken(position, '"', document, position);
        CELL_ASSERT(seekResult == seekResult::Success);

        // find its end
        size_t end = 0;
        seekResult = seekNextToken(end, '"', document, position + 1, false);
        CELL_ASSERT(seekResult == seekResult::Success);

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

        position += parseValue(value, document + position, size - position, recursionCounter);

        values.Append(value);

        CELL_ASSERT(document[position] == ',' || document[position] == ' ' || document[position] == '\n');
        position++;

        while (document[position] == ' ' || document[position] == '\n') {
            position++;
        }

        if (document[position] == '}') {
            break;
        }
    }

    recursionCounter--;
    return position;
}

Wrapped<Document*, Result> Document::Parse(const String& string) {
    if (string.IsEmpty()) {
        return Result::InvalidParameters;
    }

    const char* document = string.ToRawPointer();

    // seek to beginning
    size_t position = 0;
    seekResult seekResult = seekNextToken(position, '{', document);
    if (seekResult != seekResult::Success) {
        return Result::InvalidData;
    }

    position++;

    Collection::List<Value> values;
    uint8_t recursionCounter = 0;
    parseObject(values, document + position, string.GetSize() - position, recursionCounter);

    Value root = { .name = nullptr, .type = Type::Object, .object = nullptr, .size = values.GetCount() };

    root.object = System::AllocateMemory<Value>(values.GetCount());
    for (size_t i = 0; i < values.GetCount(); i++) {
        root.object[i] = values[i];
    }

    return new Document(root);
}

Document::~Document() {
    System::FreeMemory(this->root.object);
}

}
