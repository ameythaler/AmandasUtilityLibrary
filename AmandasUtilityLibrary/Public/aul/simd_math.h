#pragma once

#include <aul/impl/config.h>

#include <aul/impl/simd_math_types.h>

namespace aul
{
    //////////////////////////////////////////////////////////////////////////
    // vec4f
    //////////////////////////////////////////////////////////////////////////
#if AUL_USE_SSE

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

    inline vec4f vec4f_set(const float* vec)
    {
        return _mm_load_ps(vec);
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

    inline float vec4f_get_z(vec4f vec)
    {
        float retVal;
        vec4f shuffled = AUL_INTERNAL_VEC4F_REPLICATE(vec, 2);
        _mm_store_ss(&retVal, shuffled);
        return retVal;
    }

    inline float vec4f_get_w(vec4f vec)
    {
        float retVal;
        vec4f shuffled = AUL_INTERNAL_VEC4F_REPLICATE(vec, 3);
        _mm_store_ss(&retVal, shuffled);
        return retVal;
    }

    inline void vec4f_get(float* vec_array, vec4f vec)
    {
        _mm_store_ps(vec_array, vec);
    }

    inline vec4f vec4f_add(vec4f lhs, vec4f rhs)
    {
        return _mm_add_ps(lhs, rhs);
    }

    inline vec4f vec4f_sub(vec4f lhs, vec4f rhs)
    {
        return _mm_sub_ps(lhs, rhs);
    }

    inline vec4f vec4f_mul(vec4f lhs, vec4f rhs)
    {
        return _mm_mul_ps(lhs, rhs);
    }

    inline vec4f vec4f_div(vec4f lhs, vec4f rhs)
    {
        return _mm_div_ps(lhs, rhs);
    }

    inline vec4f vec4f_sqrt(vec4f vec)
    {
        return _mm_sqrt_ps(vec);
    }

    inline vec4f vec4f_dot(vec4f lhs, vec4f rhs)
    {
        vec4f multiplied = _mm_mul_ps(lhs, rhs);
        vec4f zwxy = _mm_movehl_ps(multiplied, multiplied);
        vec4f sum1 = _mm_add_ps(multiplied, zwxy);
        vec4f sum2 = _mm_hadd_ps(sum1, sum1);
        return AUL_INTERNAL_VEC4F_REPLICATE(sum2, 0);
    }

    inline vec4f vec4f_length_sq(vec4f vec)
    {
        return vec4f_dot(vec, vec);
    }

    inline vec4f vec4f_length(vec4f vec)
    {
        return _mm_sqrt_ps(vec4f_length_sq(vec));
    }

    inline vec4f vec4f_normalize(vec4f vec)
    {
        return _mm_div_ps(vec, vec4f_length(vec));
    }

    inline vec4f vec4f_cross(vec4f lhs, vec4f rhs)
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

    inline vec4f vec4f_equal_vec(vec4f lhs, vec4f rhs) // #TODO use an epsilon
    {
        return _mm_cmpeq_ps(lhs, rhs);
    }

    inline vec4f vec4f_negate(vec4f vec)
    {
        static const vec4f negate = _mm_set_ps1(-1.0f);
        return _mm_mul_ps(vec, negate);
    }
#endif // AUL_USE_SSE
}