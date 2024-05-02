// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/IO/Directory.hh>

namespace Cell::IO {

Wrapped<Directory*, Result> Directory::Open(const String& path) {
    (void)(path);

    CELL_UNIMPLEMENTED
}

Directory::~Directory() {
    (void)(this->impl);

    CELL_UNIMPLEMENTED
}

Wrapped<Collection::List<String>, Result> Directory::Enumerate(const String& filter, const bool useFullPaths) {
    (void)(filter); (void)(useFullPaths);

    CELL_UNIMPLEMENTED
}

Wrapped<Collection::List<String>, Result> Directory::EnumerateDirectories(const bool useFullPaths) {
    (void)(useFullPaths);

    CELL_UNIMPLEMENTED
}

}
