# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

# Packages a folder as an archive container.
#
# Parameters: target  (Target): Name of the target that wants this package. name    (String): Name of the resulting package. input   (Path):   Path of the
# folder to pack.
function (CellAddContentToPack target name input)
    return() # currently disabled

    if (NOT CELL_ENABLE_UTILITIES)
        return()
    endif ()

    set(output "${GENERATE_DIR}/${name}.cellpkg")

    add_custom_command(
        COMMENT "Packaging ${name}..."
        OUTPUT ${output}
        DEPENDS ${input}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND $<TARGET_FILE:CellUtilitiesContentPacker> ${input} "$<TARGET_FILE_DIR:${target}>/${name}.cellpkg"
    )

    add_custom_target(CellPackedContent.${name} ALL DEPENDS ${output})
endfunction ()
