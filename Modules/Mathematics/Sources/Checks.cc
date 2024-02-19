// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#include <Cell/Mathematics/Matrix4x4.hh>
#include <Cell/Mathematics/Vector2.hh>
#include <Cell/System/Panic.hh>
#include <Cell/Utilities/Preprocessor.hh>

namespace Cell::Mathematics {

CELL_STATIC_ASSERT(CELL_IS_STANDARD_LAYOUT(Vector2));
CELL_STATIC_ASSERT(CELL_IS_STANDARD_LAYOUT(Vector3));
CELL_STATIC_ASSERT(CELL_IS_STANDARD_LAYOUT(Quaternion));
CELL_STATIC_ASSERT(CELL_IS_STANDARD_LAYOUT(Matrix4x4));

}
