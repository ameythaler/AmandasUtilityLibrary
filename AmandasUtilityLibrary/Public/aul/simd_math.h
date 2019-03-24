#pragma once

#include "impl/config.h"

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

namespace aul
{
#if AUL_USE_SSE
    typedef __m128 vec4f;
#endif // AUL_USE_SSE

#if AUL_USE_SSE2
    typedef __m128d vec2d;
    typedef __m128i vec4i;
#endif // AUL_USE_SSE2

    //////////////////////////////////////////////////////////////////////////
    // vec4f
    //////////////////////////////////////////////////////////////////////////
#if AUL_USE_SSE
#define AUL_INTERNAL_VEC4F_MASK(x, y, z, w) (w << 6 | z << 4 | y << 2 | x)
#define AUL_INTERNAL_VEC4F_REPLICATE(v, a) _mm_shuffle_ps(v, v, AUL_INTERNAL_VEC4F_MASK(a, a, a, a))

    inline vec4f vec4f_set(float x, float y, float z, float w)
    {
        return _mm_set_ps(w, z, y, x);
    }

    inline vec4f vec4f_set(float x)
    {
        return _mm_set_ps1(x);
    }

    inline vec4f vec4f_set_zero()
    {
        return _mm_setzero_ps();
    }

    inline vec4f vec4f_set(const float* pVec)
    {
        return _mm_load_ps(pVec);
    }

    inline float vec4f_get_x(vec4f vec)
    {
        float retVal;
        _mm_store_ss(&retVal, vec);
        return retVal;
    }

    inline float vec4f_get_y(vec4f vec)
    {
        float retVal;
        vec4f shuffled = AUL_INTERNAL_VEC4F_REPLICATE(vec, 1);
        _mm_store_ss(&retVal, shuffled);
        return retVal;
    }

    inline float vec4fGetZ(vec4f vec)
    {
        float retVal;
        vec4f shuffled = AUL_INTERNAL_VEC4F_REPLICATE(vec, 2);
        _mm_store_ss(&retVal, shuffled);
        return retVal;
    }

    inline float vec4fGetW(vec4f vec)
    {
        float retVal;
        vec4f shuffled = AUL_INTERNAL_VEC4F_REPLICATE(vec, 3);
        _mm_store_ss(&retVal, shuffled);
        return retVal;
    }

    inline void vec4fGet(float* pVec, vec4f vec)
    {
        _mm_store_ps(pVec, vec);
    }

    inline vec4f vec4fAdd(vec4f lhs, vec4f rhs)
    {
        return _mm_add_ps(lhs, rhs);
    }

    inline vec4f vec4fSub(vec4f lhs, vec4f rhs)
    {
        return _mm_sub_ps(lhs, rhs);
    }

    inline vec4f vec4fMul(vec4f lhs, vec4f rhs)
    {
        return _mm_mul_ps(lhs, rhs);
    }

    inline vec4f vec4fDiv(vec4f lhs, vec4f rhs)
    {
        return _mm_div_ps(lhs, rhs);
    }

    inline vec4f vec4fSqrt(vec4f vec)
    {
        return _mm_sqrt_ps(vec);
    }

    inline vec4f vec4fDot(vec4f lhs, vec4f rhs)
    {
        vec4f multiplied = _mm_mul_ps(lhs, rhs);
        vec4f zwxy = _mm_movehl_ps(multiplied, multiplied);
        vec4f sum1 = _mm_add_ps(multiplied, zwxy);
        vec4f sum2 = _mm_hadd_ps(sum1, sum1);
        return AUL_INTERNAL_VEC4F_REPLICATE(sum2, 0);
    }

    inline vec4f vec4fLengthSq(vec4f vec)
    {
        return vec4fDot(vec, vec);
    }

    inline vec4f vec4fLength(vec4f vec)
    {
        return _mm_sqrt_ps(vec4fLengthSq(vec));
    }

    inline vec4f vec4fNormalize(vec4f vec)
    {
        return _mm_div_ps(vec, vec4fLength(vec));
    }

    inline vec4f vec4fCross(vec4f lhs, vec4f rhs)
    {
        static const vec4f negateY = _mm_set_ps(1, 1, -1, 1);
        vec4f lhsYXX = _mm_shuffle_ps(lhs, lhs, AUL_INTERNAL_VEC4F_MASK(1, 0, 0, 3));
        vec4f lhsZZY = _mm_shuffle_ps(lhs, lhs, AUL_INTERNAL_VEC4F_MASK(2, 2, 1, 3));
        vec4f rhsZZY = _mm_shuffle_ps(rhs, rhs, AUL_INTERNAL_VEC4F_MASK(2, 2, 1, 3));
        vec4f rhsYXX = _mm_shuffle_ps(rhs, rhs, AUL_INTERNAL_VEC4F_MASK(1, 0, 0, 3));
        vec4f lhMul = _mm_mul_ps(lhsYXX, rhsZZY);
        vec4f rhMul = _mm_mul_ps(lhsZZY, rhsYXX);
        vec4f retVal = _mm_sub_ps(lhMul, rhMul);
        return _mm_mul_ps(retVal, negateY);
    }

    inline vec4f vec4fEqualVec(vec4f lhs, vec4f rhs) // #TODO use an epsilon
    {
        return _mm_cmpeq_ps(lhs, rhs);
    }

    inline vec4f vec4fNegate(vec4f vec)
    {
        static const vec4f negate = _mm_set_ps1(-1.0f);
        return _mm_mul_ps(vec, negate);
    }
#endif // AUL_USE_SSE
}