// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/System/String.hh>

#include <string.h>
#include <stdio.h>

namespace Cell::System {

String String::FormatImplementation(const char* format, const StringFormatting::Data* content, const size_t count) {
    String output;

    size_t insertedArgumentCount = 0;
    for (size_t formatCharacterIndex = 0; formatCharacterIndex < strlen(format); formatCharacterIndex++) {
        if (format[formatCharacterIndex] == '%') {
            // if you manage to have larger digits, may God help you.
            char buf[200] = { 0};

            switch (content[insertedArgumentCount].type) {
            case StringFormatting::Type::Int: {
                sprintf(buf, "%lld", content[insertedArgumentCount].sInt);
                output += buf;
                break;
            }

            case StringFormatting::Type::UInt: {
                sprintf(buf, "%llu", content[insertedArgumentCount].uInt);
                output += buf;
                break;
            }

            case StringFormatting::Type::ConstCharPointer: {
                output += content[insertedArgumentCount].constCharPointer;
                break;
            }

            case StringFormatting::Type::ConstWideCharPointer: {
                output += System::String::FromPlatformWideString(content[insertedArgumentCount].constWideCharPointer).Unwrap();
                break;
            }

            case StringFormatting::Type::Address: {
                sprintf(buf, "0x%016llx", (uint64_t)(content[insertedArgumentCount].address));
                output += buf;
                break;
            }

            case StringFormatting::Type::CellString: {
                output += content[insertedArgumentCount].string;
                break;
            }

            case StringFormatting::Type::FloatingPoint: {
                sprintf(buf, "%f", content[insertedArgumentCount].floatingPoint);
                output += buf;
            }
            }

            insertedArgumentCount++;
            continue;
        }

        char stuff[2] = { format[formatCharacterIndex], '\0' };
        output += stuff;
    }

    CELL_ASSERT(insertedArgumentCount == count);
    return output;
}

}
