# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

macro (CellAddModule name)
    string(TOUPPER "CELL_ENABLE_${name_upper}_MODULE" option)

    option(${option} "Whether the ${name} module should be turned on. Defaults to ON." ON)

    if (${option})
        add_subdirectory(${MODULES_DIR}/${name})
    endif ()
endmacro ()
