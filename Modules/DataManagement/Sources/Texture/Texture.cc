// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/DataManagement/Texture.hh>
#include <Cell/Memory/Allocator.hh>

namespace Cell::DataManagement {

Texture::~Texture() {
    Memory::Free(this->rgba);
}

}
