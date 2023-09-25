# SPDX-FileCopyrightText: Copyright 2023 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(QuickJS LANGUAGES C VERSION 1.0.0)

add_library(QuickJS STATIC
    ${CMAKE_CURRENT_LIST_DIR}/cutils.h
    ${CMAKE_CURRENT_LIST_DIR}/libbf.h
    ${CMAKE_CURRENT_LIST_DIR}/libregexp-opcode.h
    ${CMAKE_CURRENT_LIST_DIR}/libregexp.h
    ${CMAKE_CURRENT_LIST_DIR}/libunicode.h
    ${CMAKE_CURRENT_LIST_DIR}/list.h
    ${CMAKE_CURRENT_LIST_DIR}/quickjs-atom.h
    ${CMAKE_CURRENT_LIST_DIR}/quickjs-opcode.h
    ${CMAKE_CURRENT_LIST_DIR}/quickjs.h

    ${CMAKE_CURRENT_LIST_DIR}/cutils.c
    ${CMAKE_CURRENT_LIST_DIR}/libbf.c
    ${CMAKE_CURRENT_LIST_DIR}/libregexp.c
    ${CMAKE_CURRENT_LIST_DIR}/libunicode.c
    ${CMAKE_CURRENT_LIST_DIR}/quickjs.c
)

target_compile_definitions(QuickJS PUBLIC "CONFIG_BIGNUM=1" "_CRT_SECURE_NO_WARNINGS=1")

target_compile_options(QuickJS PRIVATE
    -Wall
    -Werror
    -Wextra

    -Wno-zero-length-array
    -Wno-microsoft-enum-forward-reference
    -Wno-unused-parameter
    -Wno-sign-compare
    -Wgnu-label-as-value
    -Wno-pedantic
    -Wno-implicit-const-int-float-conversion
)
