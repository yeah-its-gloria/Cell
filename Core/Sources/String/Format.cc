// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Scoped.hh>
#include <Cell/String.hh>

#include <string.h>
#include <stdio.h>

namespace Cell {
using namespace StringDetails::Formatting;

String String::FormatImplementation(const char* format, const size_t length, const Data* content, const size_t count) {
    String output;

    size_t insertedArgumentCount = 0;
    for (size_t formatCharacterIndex = 0; formatCharacterIndex < length; formatCharacterIndex++) {
        if (format[formatCharacterIndex] == '%') {
            // if you manage to have larger digits, may God help you.
            char buf[200] = { 0};

            switch (content[insertedArgumentCount].type) {
            case Type::Int: {
                sprintf(buf, "%lld", content[insertedArgumentCount].sInt);
                output += buf;
                break;
            }

            case Type::UInt: {
                sprintf(buf, "%llu", content[insertedArgumentCount].uInt);
                output += buf;
                break;
            }

            case Type::ConstCharPointer: {
                output += content[insertedArgumentCount].constCharPointer;
                break;
            }

            case Type::ConstWideCharPointer: {
                output += String::FromPlatformWideString(content[insertedArgumentCount].constWideCharPointer).Unwrap();
                break;
            }

            case Type::Address: {
                // Avoids a formatting error, I hate C types so much
                sprintf(buf, "0x%016llx", (unsigned long long)(content[insertedArgumentCount].address));
                output += buf;
                break;
            }

            case Type::CellString: {
                output += content[insertedArgumentCount].string.Unwrap();
                break;
            }

            case Type::FloatingPoint: {
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
