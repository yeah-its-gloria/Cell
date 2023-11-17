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

target_compile_definitions(QuickJS PUBLIC "CONFIG_BIGNUM=1")

if(WIN32)
    target_compile_definitions(QuickJS PUBLIC "_CRT_SECURE_NO_WARNINGS=1")
endif()

# Why do I bother?
target_compile_options(QuickJS PRIVATE
    -Wall
    -Werror
    -Wextra

    -Wno-zero-length-array
    -Wno-microsoft-enum-forward-reference
    -Wno-unused-parameter
    -Wno-unused-variable
    -Wno-unused-but-set-variable
    -Wno-sign-compare
    -Wno-gnu-label-as-value
    -Wno-pedantic
    -Wno-implicit-const-int-float-conversion

    -Wno-sign-conversion
    -Wno-implicit-int-conversion
    -Wno-shorten-64-to-32
    -Wno-reserved-macro-identifier
    -Wno-unsafe-buffer-usage
    -Wno-implicit-fallthrough
    -Wno-cast-align
    -Wno-cast-qual
    -Wno-reserved-identifier
    -Wno-c++-compat
    -Wno-shadow
    -Wno-missing-prototypes
    -Wno-format-nonliteral
    -Wno-used-but-marked-unused
    -Wno-cast-function-type-strict
    -Wno-unused-macros
    -Wno-cast-function-type
    -Wno-declaration-after-statement
    -Wno-duplicate-enum
    -Wno-double-promotion
    -Wno-bad-function-cast
    -Wno-switch-enum
    -Wno-unreachable-code-break
    -Wno-float-conversion
    -Wno-covered-switch-default
    -Wno-implicit-int-float-conversion
    -Wno-float-equal
    -Wno-implicit-float-conversion
    -Wno-conditional-uninitialized
    -Wno-extra-semi-stmt
    -Wno-unreachable-code-return
    -Wno-unreachable-code
    -Wno-shift-sign-overflow

    -Wno-unknown-warning-option
)
