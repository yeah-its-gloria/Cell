# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

project(zlib LANGUAGES C VERSION 1.0.0 DESCRIPTION "zlib-ng")

option(ZLIB_DEBUG "Whether to enable zlib debug output" OFF)

set(INCLUDES_DIR ${CMAKE_CURRENT_LIST_DIR}/Includes)
set(SOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/Sources)

add_library(zlib STATIC
    ${INCLUDES_DIR}/zconf-ng.h
    ${INCLUDES_DIR}/zlib-ng.h
    ${INCLUDES_DIR}/zlib_name_mangling-ng.h

    ${SOURCES_DIR}/adler32_p.h
    ${SOURCES_DIR}/arch_functions.h
    ${SOURCES_DIR}/chunkset_tpl.h
    ${SOURCES_DIR}/compare256_rle.h
    ${SOURCES_DIR}/crc32.h
    ${SOURCES_DIR}/crc32_braid_comb_p.h
    ${SOURCES_DIR}/crc32_braid_p.h
    ${SOURCES_DIR}/crc32_braid_tbl.h
    ${SOURCES_DIR}/deflate.h
    ${SOURCES_DIR}/deflate_p.h
    ${SOURCES_DIR}/fallback_builtins.h
    ${SOURCES_DIR}/functable.h
    ${SOURCES_DIR}/inffast_tpl.h
    ${SOURCES_DIR}/inffixed_tbl.h
    ${SOURCES_DIR}/inflate.h
    ${SOURCES_DIR}/inflate_p.h
    ${SOURCES_DIR}/inftrees.h
    ${SOURCES_DIR}/insert_string_tpl.h
    ${SOURCES_DIR}/match_tpl.h
    ${SOURCES_DIR}/trees.h
    ${SOURCES_DIR}/trees_emit.h
    ${SOURCES_DIR}/trees_tbl.h
    ${SOURCES_DIR}/zbuild.h
    ${SOURCES_DIR}/zendian.h
    ${SOURCES_DIR}/zutil.h
    ${SOURCES_DIR}/zutil_p.h

    ${SOURCES_DIR}/adler32.c
    ${SOURCES_DIR}/compress.c
    ${SOURCES_DIR}/crc32.c
    ${SOURCES_DIR}/crc32_braid_comb.c
    ${SOURCES_DIR}/deflate.c
    ${SOURCES_DIR}/deflate_fast.c
    ${SOURCES_DIR}/deflate_huff.c
    ${SOURCES_DIR}/deflate_medium.c
    ${SOURCES_DIR}/deflate_quick.c
    ${SOURCES_DIR}/deflate_rle.c
    ${SOURCES_DIR}/deflate_slow.c
    ${SOURCES_DIR}/deflate_stored.c
    ${SOURCES_DIR}/infback.c
    ${SOURCES_DIR}/inflate.c
    ${SOURCES_DIR}/inftrees.c
    ${SOURCES_DIR}/insert_string.c
    ${SOURCES_DIR}/insert_string_roll.c
    ${SOURCES_DIR}/trees.c
    ${SOURCES_DIR}/uncompr.c
    ${SOURCES_DIR}/zutil.c

    ${SOURCES_DIR}/generic/chunk_permute_table.h
    ${SOURCES_DIR}/generic/generic_functions.h

    ${SOURCES_DIR}/generic/adler32_c.c
    ${SOURCES_DIR}/generic/adler32_fold_c.c
    ${SOURCES_DIR}/generic/chunkset_c.c
    ${SOURCES_DIR}/generic/compare256_c.c
    ${SOURCES_DIR}/generic/crc32_braid_c.c
    ${SOURCES_DIR}/generic/crc32_fold_c.c
    ${SOURCES_DIR}/generic/slide_hash_c.c
)

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AMD64" OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    target_sources(zlib PRIVATE
        ${SOURCES_DIR}/x86/adler32_avx2_p.h
        ${SOURCES_DIR}/x86/adler32_ssse3_p.h
        ${SOURCES_DIR}/x86/x86_functions.h
        ${SOURCES_DIR}/x86/x86_intrins.h

        ${SOURCES_DIR}/x86/adler32_avx2.c
        ${SOURCES_DIR}/x86/adler32_sse42.c
        ${SOURCES_DIR}/x86/adler32_ssse3.c
        ${SOURCES_DIR}/x86/chunkset_avx2.c
        ${SOURCES_DIR}/x86/chunkset_sse2.c
        ${SOURCES_DIR}/x86/chunkset_ssse3.c
        ${SOURCES_DIR}/x86/compare256_avx2.c
        ${SOURCES_DIR}/x86/compare256_sse2.c
        ${SOURCES_DIR}/x86/slide_hash_avx2.c
        ${SOURCES_DIR}/x86/slide_hash_sse2.c
    )

    target_compile_definitions(zlib PRIVATE
        WITH_AVX2
        WITH_SSE2
        WITH_SSSE3
        WITH_SSE42

        X86_AVX512VNNI
        X86_AVX512
        X86_SSE42
        X86_AVX2
        X86_SSSE3
        X86_SSE2

        HAVE_BUILTIN_CTZ

        X86_FEATURES
    )

    target_compile_options(zlib PRIVATE -mavx2 -mfma -march=cascadelake)
elseif (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
    target_sources(zlib PRIVATE
        ${SOURCES_DIR}/arm/acle_intrins.h
        ${SOURCES_DIR}/arm/arm_features.h
        ${SOURCES_DIR}/arm/arm_functions.h
        ${SOURCES_DIR}/arm/neon_intrins.h

        ${SOURCES_DIR}/arm/adler32_neon.c
        ${SOURCES_DIR}/arm/arm_features.c
        ${SOURCES_DIR}/arm/chunkset_neon.c
        ${SOURCES_DIR}/arm/compare256_neon.c
        ${SOURCES_DIR}/arm/crc32_acle.c
        ${SOURCES_DIR}/arm/slide_hash_armv6.c
        ${SOURCES_DIR}/arm/slide_hash_neon.c
    )

    target_compile_definitions(zlib PRIVATE ARM_FEATURES ARM_NEON ARM_SIMD)
    target_compile_options(zlib PRIVATE -mfloat-abi=hard)
else ()
    message(AUTHOR_WARNING "No zlib acceleration available for ${CMAKE_SYSTEM_PROCESSOR}")
endif ()

if (ZLIB_DEBUG)
    target_compile_definitions(zlib PRIVATE "$<$<CONFIG:Debug>:ZLIB_DEBUG>")
endif ()

target_compile_definitions(zlib PRIVATE DISABLE_RUNTIME_CPU_DETECTION)

target_compile_options(zlib PRIVATE
    -Wno-constant-logical-operand
    -Wno-language-extension-token
    -Wno-empty-translation-unit
)

target_include_directories(zlib PUBLIC ${INCLUDES_DIR})
target_link_libraries(zlib PUBLIC CellCore)

cell_target_bootstrap(zlib)
