// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/IO/File.hh>

namespace Cell::DataManagement {

class Archive : public Object {
public:
    Archive(IO::File* CELL_NONNULL file);
    ~Archive();

private:
    IO::File* file;
};

}
