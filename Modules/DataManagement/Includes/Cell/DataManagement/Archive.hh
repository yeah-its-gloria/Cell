// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/IO/File.hh>

namespace Cell::DataManagement {

struct CELL_PACKED(1) ArchiveHeader {
    uint32_t elements;
};

struct CELL_PACKED(1) ArchiveEntry {
    uint8_t name[20];
    uint32_t size;
};

class Archive : public NoCopyObject {
public:
    Archive(IO::File* CELL_NONNULL file);
    ~Archive();

private:
    IO::File* file;
};

}
