# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

include(FindPackageHandleStandardArgs)

find_program(cargo_PROGRAM cargo REQUIRED)
find_program(cbindgen_PROGRAM cbindgen REQUIRED)

find_package_handle_standard_args(RustTools DEFAULT_MSG cargo_PROGRAM cbindgen_PROGRAM)
