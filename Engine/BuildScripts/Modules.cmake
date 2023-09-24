# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

# Inlines an option that can disable parts of the engine build. Meant mostly for modules.
#
# Parameters: name        (String): Name of the option. It should be fully qualified, and prefixed with "CELL_", unless it is an external third party module.
# path        (Path):   Path to the module. description (String): Description for the option.
macro (CellAddModule name path description)
    option(${name} ${description} ON)

    if (${name})
        add_subdirectory(${path})
    endif ()
endmacro ()
