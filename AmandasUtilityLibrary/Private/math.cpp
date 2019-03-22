#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "aul/math.h"
#include <limits.h>

namespace aul
{
#define AUL_INTERNAL_EXPLICIT_DEF_SCALAR(T, zero, one, two, four) const T scalar<T>::ZERO = zero; \
    const T scalar<T>::ONE = one; \
    const T scalar<T>::TWO = two; \
    const T scalar<T>::FOUR = four; \
    const T scalar<T>::EPSILON = std::numeric_limits<T>::epsilon(); \
    const T scalar<T>::PI = (T)M_PI; \
    const T scalar<T>::PI_2 = (T)M_PI * two; \
    const T scalar<T>::PI_4 = (T)M_PI * four; \
    const T scalar<T>::PI_OVER_2 = (T)M_PI / two; \
    const T scalar<T>::PI_OVER_4 = (T)M_PI / four; \
    const T scalar<T>::MAX = std::numeric_limits<T>::max(); \
    bool scalar<T>::equal(T lhs, T rhs) \
    { \
        return abs(lhs - rhs) <= EPSILON; \
    } \
    bool scalar<T>::not_equal(T lhs, T rhs) \
    { \
        return abs(lhs - rhs) > EPSILON; \
    } \
    template struct scalar<T>

#define AUL_INTERNAL_EXPLICIT_DEF_SCALAR_INT(T, zero, one, two, four) const T scalar_int<T>::ZERO = zero; \
    const T scalar_int<T>::ONE = one; \
    const T scalar_int<T>::TWO = two; \
    const T scalar_int<T>::FOUR = four; \
    const T scalar_int<T>::MAX = std::numeric_limits<T>::max(); \
    bool scalar_int<T>::equal(T lhs, T rhs) \
    { \
        return lhs == rhs; \
    } \
    bool scalar_int<T>::not_equal(T lhs, T rhs) \
    { \
        return lhs != rhs; \
    } \
    template struct scalar_int<T>

    AUL_INTERNAL_EXPLICIT_DEF_SCALAR(float, 0.0f, 1.0f, 2.0f, 4.0f);
    AUL_INTERNAL_EXPLICIT_DEF_SCALAR(double, 0.0, 1.0, 2.0, 4.0);
    AUL_INTERNAL_EXPLICIT_DEF_SCALAR_INT(int32, 0, 1, 2, 4);
    AUL_INTERNAL_EXPLICIT_DEF_SCALAR_INT(uint32, 0u, 1u, 2u, 4u);
    AUL_INTERNAL_EXPLICIT_DEF_SCALAR_INT(int64, 0, 1, 2, 4);
    AUL_INTERNAL_EXPLICIT_DEF_SCALAR_INT(uint64, 0u, 1u, 2u, 4u);
    AUL_INTERNAL_EXPLICIT_DEF_SCALAR_INT(int16, 0, 1, 2, 4);
    AUL_INTERNAL_EXPLICIT_DEF_SCALAR_INT(uint16, 0u, 1u, 2u, 4u);
    AUL_INTERNAL_EXPLICIT_DEF_SCALAR_INT(int8, 0, 1, 2, 4);
    AUL_INTERNAL_EXPLICIT_DEF_SCALAR_INT(uint8, 0u, 1u, 2u, 4u);

#undef AUL_INTERNAL_EXPLICIT_DEF_SCALAR
#undef AUL_INTERNAL_EXPLICIT_DEF_SCALAR_INT
}