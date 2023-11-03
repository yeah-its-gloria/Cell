// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/DataManagement/Archive.hh>

namespace Cell::DataManagement {

Archive::Archive(IO::File* file) : file(file) {
    // TODO: read stuff
}

Archive::~Archive() {
    delete this->file;
}

}
