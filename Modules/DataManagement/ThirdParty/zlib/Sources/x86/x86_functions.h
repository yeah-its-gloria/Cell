/* x86_functions.h -- x86 implementations for arch-specific functions.
 * Copyright (C) 2013 Intel Corporation Jim Kukunas
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#pragma once

#ifdef X86_SSE2
uint32_t chunksize_sse2(void);
uint8_t* chunkmemset_safe_sse2(uint8_t *out, unsigned dist, unsigned len, unsigned left);
#  ifdef HAVE_BUILTIN_CTZ
    uint32_t compare256_sse2(const uint8_t *src0, const uint8_t *src1);
    uint32_t longest_match_sse2(deflate_state *const s, Pos cur_match);
    uint32_t longest_match_slow_sse2(deflate_state *const s, Pos cur_match);
    void slide_hash_sse2(deflate_state *s);
#  endif
    void inflate_fast_sse2(PREFIX3(stream)* strm, uint32_t start);
#endif

#ifdef X86_SSSE3
uint32_t adler32_ssse3(uint32_t adler, const uint8_t *buf, size_t len);
uint8_t* chunkmemset_safe_ssse3(uint8_t *out, unsigned dist, unsigned len, unsigned left);
void inflate_fast_ssse3(PREFIX3(stream) *strm, uint32_t start);
#endif

#ifdef X86_SSE42
uint32_t adler32_fold_copy_sse42(uint32_t adler, uint8_t *dst, const uint8_t *src, size_t len);
#endif

#ifdef X86_AVX2
uint32_t adler32_avx2(uint32_t adler, const uint8_t *buf, size_t len);
uint32_t adler32_fold_copy_avx2(uint32_t adler, uint8_t *dst, const uint8_t *src, size_t len);
uint32_t chunksize_avx2(void);
uint8_t* chunkmemset_safe_avx2(uint8_t *out, unsigned dist, unsigned len, unsigned left);
#  ifdef HAVE_BUILTIN_CTZ
    uint32_t compare256_avx2(const uint8_t *src0, const uint8_t *src1);
    uint32_t longest_match_avx2(deflate_state *const s, Pos cur_match);
    uint32_t longest_match_slow_avx2(deflate_state *const s, Pos cur_match);
    void slide_hash_avx2(deflate_state *s);
#  endif
    void inflate_fast_avx2(PREFIX3(stream)* strm, uint32_t start);
#endif

// X86 - SSE2
#  if (defined(X86_SSE2) && defined(__SSE2__)) || defined(__x86_64__) || defined(_M_X64) || defined(X86_NOCHECK_SSE2)
#    undef native_chunkmemset_safe
#    define native_chunkmemset_safe chunkmemset_safe_sse2
#    undef native_chunksize
#    define native_chunksize chunksize_sse2
#    undef native_inflate_fast
#    define native_inflate_fast inflate_fast_sse2
#    undef native_slide_hash
#    define native_slide_hash slide_hash_sse2
#    ifdef HAVE_BUILTIN_CTZ
#      undef native_compare256
#      define native_compare256 compare256_sse2
#      undef native_longest_match
#      define native_longest_match longest_match_sse2
#      undef native_longest_match_slow
#      define native_longest_match_slow longest_match_slow_sse2
#    endif
#  endif

// X86 - SSSE3
#  if defined(X86_SSSE3) && defined(__SSSE3__)
#    undef native_adler32
#    define native_adler32 adler32_ssse3
#    undef native_chunkmemset_safe
#    define native_chunkmemset_safe chunkmemset_safe_ssse3
#    undef native_inflate_fast
#    define native_inflate_fast inflate_fast_ssse3
#  endif

// X86 - SSE4.2
#  if defined(X86_SSE42) && defined(__SSE4_2__)
#    undef native_adler32_fold_copy
#    define native_adler32_fold_copy adler32_fold_copy_sse42
#  endif

// X86 - AVX
#  if defined(X86_AVX2) && defined(__AVX2__)
#    undef native_adler32
#    define native_adler32 adler32_avx2
#    undef native_adler32_fold_copy
#    define native_adler32_fold_copy adler32_fold_copy_avx2
#    undef native_chunkmemset_safe
#    define native_chunkmemset_safe chunkmemset_safe_avx2
#    undef native_chunksize
#    define native_chunksize chunksize_avx2
#    undef native_inflate_fast
#    define native_inflate_fast inflate_fast_avx2
#    undef native_slide_hash
#    define native_slide_hash slide_hash_avx2
#    ifdef HAVE_BUILTIN_CTZ
#      undef native_compare256
#      define native_compare256 compare256_avx2
#      undef native_longest_match
#      define native_longest_match longest_match_avx2
#      undef native_longest_match_slow
#      define native_longest_match_slow longest_match_slow_avx2
#    endif
#  endif
