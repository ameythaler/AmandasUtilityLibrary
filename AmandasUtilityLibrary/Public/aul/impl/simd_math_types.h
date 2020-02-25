#pragma once

#include <aul/impl/config.h>

#if AUL_USE_SSE
#include <xmmintrin.h> // SSE
#endif // AUL_USE_SSE

#if AUL_USE_SSE2
#include <emmintrin.h> // SSE2
#endif // AUL_USE_SSE2

#if AUL_USE_SSE3
#include <pmmintrin.h> // SSE3
#endif // AUL_USE_SSE3

#if AUL_USE_SSSE3
#include <tmmintrin.h> // SSSE3
#endif // AUL_USE_SSSE3

#if AUL_USE_SSE4
#include <smmintrin.h> // SSE4.1
#include <nmmintrin.h> // SSE4.2
#endif // AUL_USE_SSE4

#if AUL_USE_AVX || AUL_USE_AVX2 || AUL_USE_AVX_512
#include <immintrin.h> // AVX, AVX2, & AVX-512
#endif // AUL_USE_AVX || AUL_USE_AVX2 || AUL_USE_AVX_512

#if AUL_USE_SSE
#define AUL_INTERNAL_VEC4F_MASK(x, y, z, w) (w << 6 | z << 4 | y << 2 | x)
#define AUL_INTERNAL_VEC4F_REPLICATE(v, a) _mm_shuffle_ps(v, v, AUL_INTERNAL_VEC4F_MASK(a, a, a, a))
#endif // AUL_USE_SSE

namespace aul
{
#if AUL_USE_SSE
    typedef __m128 vec4f;
#endif // AUL_USE_SSE

#if AUL_USE_SSE2
    typedef __m128d vec2d;
    typedef __m128i vec4i;
#endif // AUL_USE_SSE2
}