// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/Wrapped.hh>
#include <Cell/DataManagement/Result.hh>
#include <Cell/Memory/Block.hh>

namespace Cell::DataManagement {

// Represents a 3D model.
class Model : public Object {
public:
    // Decodes model data from a glTF blob.
    CELL_FUNCTION static Wrapped<Model*, Result> FromGLTF(const Memory::IBlock& block);

    // Cleans up the model contained and destructs the instance.
    CELL_FUNCTION ~Model();

private:
    CELL_FUNCTION_INTERNAL Model() { }

    // ...
};

}
