// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/DataManagement/Archive.hh>

namespace Cell::DataManagement {

struct CELL_PACKED(1) ArchiveHeader {
    uint32_t entries;
};

struct CELL_PACKED(1) ArchiveEntry {
    uint8_t name[20];
    uint32_t size;
};

Archive::Archive(IO::File* file) : file(file) {
    // TODO: read stuff
}

Archive::~Archive() {
    delete this->file;
}

}
