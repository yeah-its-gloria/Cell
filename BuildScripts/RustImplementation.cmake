# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

find_package(RustTools REQUIRED)

set(CELL_RUST_TRIPLE "" CACHE STRING "Custom Rust compile triple")

function(CellAddRustImplementation target crate)
    set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/RustImplementation)
    set(BINARY_DIR ${GENERATE_DIR}/Rust/${target})

    if (NOT CELL_RUST_TRIPLE STREQUAL "")
        set(rust_triple_path "${CELL_RUST_TRIPLE}/")
        set(rust_triple_args --target ${CELL_RUST_TRIPLE})
    endif ()

    make_directory("${BINARY_DIR}/Includes")

    if (WIN32)
        set(library_output ${BINARY_DIR}/Build/${rust_triple_path}$<IF:$<CONFIG:Debug>,debug,release>/${crate}.lib)
    elseif (UNIX)
        set(library_output ${BINARY_DIR}/Build/${rust_triple_path}$<IF:$<CONFIG:Debug>,debug,release>/lib${crate}.a)
    else ()
        message(FATAL_ERROR "Platform unsupported")
    endif ()

    if (NOT EXISTS ${SOURCE_DIR})
        message(FATAL_ERROR "Failed to find the Rust sources for ${target}.")
    elseif (NOT EXISTS ${SOURCE_DIR}/Cargo.toml)
        message(FATAL_ERROR "Failed to find the cargo configuration for ${target}.")
    elseif (NOT EXISTS ${SOURCE_DIR}/cbindgen.toml)
        message(FATAL_ERROR "Failed to find the cbindgen configuration for ${target}.")
    endif ()

    set(header_output ${BINARY_DIR}/Includes/${target}RustImplementation.hh)

    add_custom_command(
        COMMENT "Building Rust objects for ${target}..."
        OUTPUT ${library_output}
        DEPENDS "${SOURCE_DIR}/Cargo.toml" "${SOURCE_DIR}/src/lib.rs"
        WORKING_DIRECTORY ${SOURCE_DIR}
        COMMAND ${cargo_PROGRAM} build $<$<NOT:$<CONFIG:Debug>>:--release> --target-dir \"${BINARY_DIR}/Build\" --quiet ${rust_triple_args}
    )

    add_custom_command(
        COMMENT "Generating Rust headers for ${target}..."
        OUTPUT ${header_output}
        DEPENDS ${library_output} "${SOURCE_DIR}/cbindgen.toml"
        WORKING_DIRECTORY ${SOURCE_DIR}
        COMMAND ${cbindgen_PROGRAM} --config cbindgen.toml --crate ${crate} --output ${header_output}
    )

    add_custom_target(${target}RustImpl ALL DEPENDS ${library_output} ${header_output})
    add_dependencies(${target} ${target}RustImpl)

    target_include_directories(${target} PRIVATE "${BINARY_DIR}/Includes")
    target_link_libraries(${target} PRIVATE ${library_output})

    set_target_properties(${target} PROPERTIES ADDITIONAL_CLEAN_FILES ${BINARY_DIR}/Build)
endfunction()
