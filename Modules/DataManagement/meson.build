# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

module_datamanagement_zlib_includes = include_directories('ThirdParty/zlib/Includes')

module_datamanagement_zlib_sources = [
    'ThirdParty/zlib/Sources/adler32.c',
    'ThirdParty/zlib/Sources/compress.c',
    'ThirdParty/zlib/Sources/crc32.c',
    'ThirdParty/zlib/Sources/crc32_braid_comb.c',
    'ThirdParty/zlib/Sources/deflate.c',
    'ThirdParty/zlib/Sources/deflate_fast.c',
    'ThirdParty/zlib/Sources/deflate_huff.c',
    'ThirdParty/zlib/Sources/deflate_medium.c',
    'ThirdParty/zlib/Sources/deflate_quick.c',
    'ThirdParty/zlib/Sources/deflate_rle.c',
    'ThirdParty/zlib/Sources/deflate_slow.c',
    'ThirdParty/zlib/Sources/deflate_stored.c',
    'ThirdParty/zlib/Sources/infback.c',
    'ThirdParty/zlib/Sources/inflate.c',
    'ThirdParty/zlib/Sources/inftrees.c',
    'ThirdParty/zlib/Sources/insert_string.c',
    'ThirdParty/zlib/Sources/insert_string_roll.c',
    'ThirdParty/zlib/Sources/trees.c',
    'ThirdParty/zlib/Sources/uncompr.c',
    'ThirdParty/zlib/Sources/zutil.c',

    'ThirdParty/zlib/Sources/generic/adler32_c.c',
    'ThirdParty/zlib/Sources/generic/adler32_fold_c.c',
    'ThirdParty/zlib/Sources/generic/chunkset_c.c',
    'ThirdParty/zlib/Sources/generic/compare256_c.c',
    'ThirdParty/zlib/Sources/generic/crc32_braid_c.c',
    'ThirdParty/zlib/Sources/generic/crc32_fold_c.c',
    'ThirdParty/zlib/Sources/generic/slide_hash_c.c',
]

module_datamanagement_zlib_args = [
    '-DHAVE_ATTRIBUTE_ALIGNED=1',
    '-DDISABLE_RUNTIME_CPU_DETECTION=1',

    '-Wno-constant-logical-operand',
    '-Wno-language-extension-token',
    '-Wno-empty-translation-unit'
]

if host_machine.cpu_family() == 'aarch64'
    module_datamanagement_zlib_sources += [
        'ThirdParty/zlib/Sources/arm/adler32_neon.c',
        'ThirdParty/zlib/Sources/arm/chunkset_neon.c',
        'ThirdParty/zlib/Sources/arm/compare256_neon.c',
        'ThirdParty/zlib/Sources/arm/crc32_acle.c',
        'ThirdParty/zlib/Sources/arm/slide_hash_neon.c'
    ]

    module_datamanagement_zlib_args += [
        '-DARM_FEATURES=1',
        '-DARM_ACLE=1',
        '-DARM_NEON=1',
        '-DARM_NEON_HASLD4=1',
        '-DHAVE_ARM_ACLE_H=1',
        '-DHAVE_BUILTIN_CTZLL=1',

        '-Wno-shorten-64-to-32'
    ]
elif host_machine.cpu_family() == 'x86_64'
    module_datamanagement_zlib_sources += [
        'ThirdParty/zlib/Sources/x86/adler32_avx2_p.h',
        'ThirdParty/zlib/Sources/x86/adler32_ssse3_p.h',
        'ThirdParty/zlib/Sources/x86/x86_functions.h',
        'ThirdParty/zlib/Sources/x86/x86_intrins.h',

        'ThirdParty/zlib/Sources/x86/adler32_avx2.c',
        'ThirdParty/zlib/Sources/x86/adler32_sse42.c',
        'ThirdParty/zlib/Sources/x86/adler32_ssse3.c',
        'ThirdParty/zlib/Sources/x86/chunkset_avx2.c',
        'ThirdParty/zlib/Sources/x86/chunkset_sse2.c',
        'ThirdParty/zlib/Sources/x86/chunkset_ssse3.c',
        'ThirdParty/zlib/Sources/x86/compare256_avx2.c',
        'ThirdParty/zlib/Sources/x86/compare256_sse2.c',
        'ThirdParty/zlib/Sources/x86/slide_hash_avx2.c',
        'ThirdParty/zlib/Sources/x86/slide_hash_sse2.c'
    ]

    module_datamanagement_zlib_args += [
        '-DX86_FEATURES=1',
        '-DX86_AVX512VNNI=1',
        '-DX86_AVX512=1',
        '-DX86_SSE42=1',
        '-DX86_AVX2=1',
        '-DX86_SSSE3=1',
        '-DX86_SSE2=1',

        '-DHAVE_BUILTIN_CTZ=1',
        '-DWITH_AVX2=1',
        '-DWITH_SSE2=1',
        '-DWITH_SSSE3=1',
        '-DWITH_SSE42=1',

        '-mavx2',
        '-mfma',
        '-march=cascadelake'
    ]
endif

module_datamanagement_zlib = static_library('zlib',
    sources: module_datamanagement_zlib_sources,
    include_directories: module_datamanagement_zlib_includes,
    c_args: module_datamanagement_zlib_args,

    dependencies: core
)

module_datamanagement_includes = include_directories('Includes')

module_datamanagement_sources = [
    'Sources/Archive.cc',
    'Sources/Base64.cc',
    'Sources/Checksum.cc',
    'Sources/JSON.cc',
    'Sources/zlib.cc',

    'Sources/Model/FromGLTF.cc',
    'Sources/Model/Model.cc',

    'Sources/Texture/FromPNG.cc',
    'Sources/Texture/Texture.cc',

    'Sources/HTTP/Connection.cc',
    'Sources/HTTP/Request.cc'
]

module_datamanagement_defines = [
    '-DCELL_MODULES_DATAMANAGEMENT_AVAILABLE=1'
]

module_datamanagement_library = shared_library('CellDataManagement',
    sources: module_datamanagement_sources,
    include_directories: [ module_datamanagement_includes, module_datamanagement_zlib_includes ],
    link_with: module_datamanagement_zlib,
    dependencies: core
)

module_datamanagement = declare_dependency(
    compile_args: module_datamanagement_defines,
    include_directories: module_datamanagement_includes,
    link_with: module_datamanagement_library,
    dependencies: core
)

test('glTF', executable('CellDataManagementTestglTF', sources: 'Tests/glTF.cc', dependencies: [ core, core_bootstrapper, module_datamanagement ], win_subsystem: 'console'))
test('HTTP', executable('CellDataManagementTestHTTP', sources: 'Tests/HTTP.cc', dependencies: [ core, core_bootstrapper, module_datamanagement ], win_subsystem: 'console'))
test('JSON', executable('CellDataManagementTestJSON', sources: 'Tests/JSON.cc', dependencies: [ core, core_bootstrapper, module_datamanagement ], win_subsystem: 'console'))
test('PNG',  executable('CellDataManagementTestPNG',  sources: 'Tests/PNG.cc',  dependencies: [ core, core_bootstrapper, module_datamanagement ], win_subsystem: 'console'))

if get_option('utilities')
    executable('CellDataManagementUtilitiesContentPacker',  sources: 'Utilities/ContentPacker/Main.cc', dependencies: [ core, core_bootstrapper, module_datamanagement ], win_subsystem: 'console')
endif
