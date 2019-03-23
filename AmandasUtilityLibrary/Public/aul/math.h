#pragma once

#include "aul/types.h"
#include <math.h>
#include <algorithm>

namespace aul
{
    template<typename T> struct scalar;
    template<typename T> struct scalar_int;
    typedef scalar<float> scalarf;
    typedef scalar<double> scalard;
    typedef scalar_int<int32> scalari32;
    typedef scalar_int<uint32> scalaru32;
    typedef scalar_int<int64> scalari64;
    typedef scalar_int<uint64> scalaru64;
    typedef scalar_int<int16> scalari16;
    typedef scalar_int<uint16> scalaru16;
    typedef scalar_int<int8> scalari8;
    typedef scalar_int<uint8> scalaru8;

#define AUL_INTERNAL_TRIG(x, y) inline float x(float val) { return y ## f(val); } \
inline double x(double val) { return y(val); }

    AUL_INTERNAL_TRIG(sin, sin);
    AUL_INTERNAL_TRIG(cos, cos);
    AUL_INTERNAL_TRIG(tan, tan);
    AUL_INTERNAL_TRIG(asin, asin);
    AUL_INTERNAL_TRIG(acos, acos);
    AUL_INTERNAL_TRIG(atan, atan);

#undef AUL_INTERNAL_TRIG

    inline float abs(float val) { return ::abs(val); }
    inline double abs(double val) { return ::abs(val); }

#define AUL_INTERNAL_INT_ABS(x) inline x abs(x val) { return (x)::abs(val); }
#define AUL_INTERNAL_INT_ABS_UNSIGNED(x) inline x abs(x val) { return val; } // unsigned values are already positive - this is necessary only for templated constructs

    AUL_INTERNAL_INT_ABS(int32);
    AUL_INTERNAL_INT_ABS_UNSIGNED(uint32);
    AUL_INTERNAL_INT_ABS(int64);
    AUL_INTERNAL_INT_ABS_UNSIGNED(uint64);
    AUL_INTERNAL_INT_ABS(int16);
    AUL_INTERNAL_INT_ABS_UNSIGNED(uint16);
    AUL_INTERNAL_INT_ABS(int8);
    AUL_INTERNAL_INT_ABS_UNSIGNED(uint8);

#undef AUL_INTERNAL_INT_ABS
#undef AUL_INTERNAL_INT_ABS_UNSIGNED

    inline float sqrt(float val) { return ::sqrtf(val); }
    inline double sqrt(double val) { return ::sqrt(val); }

#define AUL_INTERNAL_INT_SQRT32(x) inline x sqrt(x val) { return (x)::sqrtf((float)val); }
#define AUL_INTERNAL_INT_SQRT64(x) inline x sqrt(x val) { return (x)::sqrt((double)val); }

    AUL_INTERNAL_INT_SQRT32(int32);
    AUL_INTERNAL_INT_SQRT32(uint32);
    AUL_INTERNAL_INT_SQRT64(int64);
    AUL_INTERNAL_INT_SQRT64(uint64);
    AUL_INTERNAL_INT_SQRT32(int16);
    AUL_INTERNAL_INT_SQRT32(uint16);
    AUL_INTERNAL_INT_SQRT32(int8);
    AUL_INTERNAL_INT_SQRT32(uint8);

#undef AUL_INTERNAL_INT_SQRT32
#undef AUL_INTERNAL_INT_SQRT64

#define AUL_INTERNAL_MAX_FUNC(x) inline x max(x a, x b) { return (a > b) ? a : b; }
#define AUL_INTERNAL_MIN_FUNC(x) inline x min(x a, x b) { return (a < b) ? a : b; }

    AUL_INTERNAL_MAX_FUNC(int32);
    AUL_INTERNAL_MAX_FUNC(uint32);
    AUL_INTERNAL_MAX_FUNC(int64);
    AUL_INTERNAL_MAX_FUNC(uint64);
    AUL_INTERNAL_MAX_FUNC(int16);
    AUL_INTERNAL_MAX_FUNC(uint16);
    AUL_INTERNAL_MAX_FUNC(int8);
    AUL_INTERNAL_MAX_FUNC(uint8);
    AUL_INTERNAL_MAX_FUNC(float);
    AUL_INTERNAL_MAX_FUNC(double);

    AUL_INTERNAL_MIN_FUNC(int32);
    AUL_INTERNAL_MIN_FUNC(uint32);
    AUL_INTERNAL_MIN_FUNC(int64);
    AUL_INTERNAL_MIN_FUNC(uint64);
    AUL_INTERNAL_MIN_FUNC(int16);
    AUL_INTERNAL_MIN_FUNC(uint16);
    AUL_INTERNAL_MIN_FUNC(int8);
    AUL_INTERNAL_MIN_FUNC(uint8);
    AUL_INTERNAL_MIN_FUNC(float);
    AUL_INTERNAL_MIN_FUNC(double);

#undef AUL_INTERNAL_MAX_FUNC
#undef AUL_INTERNAL_MIN_FUNC

    template<typename T>
    struct scalar
    {
        static const T ZERO;
        static const T ONE;
        static const T TWO;
        static const T FOUR;
        static const T EPSILON;
        static const T PI;
        static const T PI_2;
        static const T PI_4;
        static const T PI_OVER_2;
        static const T PI_OVER_4;
        static const T MAX;
        static const T DEG_TO_RAD;
        static const T RAD_TO_DEG;
        static bool equal(T lhs, T rhs);
        static bool not_equal(T lhs, T rhs);
    };

    template<typename T>
    struct scalar_int
    {
        static const T ZERO;
        static const T ONE;
        static const T TWO;
        static const T FOUR;
        static const T MAX;
        static bool equal(T lhs, T rhs);
        static bool not_equal(T lhs, T rhs);
    };

#define AUL_INTERNAL_EXPLICIT_DEC_SCALAR(T) template<> \
struct scalar<T> \
{ \
    static const T ZERO; \
    static const T ONE; \
    static const T TWO; \
    static const T FOUR; \
    static const T EPSILON; \
    static const T PI; \
    static const T PI_2; \
    static const T PI_4; \
    static const T PI_OVER_2; \
    static const T PI_OVER_4; \
    static const T MAX; \
    static const T DEG_TO_RAD; \
    static const T RAD_TO_DEG; \
    static bool equal(T lhs, T rhs); \
    static bool not_equal(T lhs, T rhs); \
}; \
extern template struct scalar<T>

#define AUL_INTERNAL_EXPLICIT_DEC_SCALAR_INT(T) template<> \
struct scalar_int<T> \
{ \
    static const T ZERO; \
    static const T ONE; \
    static const T TWO; \
    static const T FOUR; \
    static const T MAX; \
    static bool equal(T lhs, T rhs); \
    static bool not_equal(T lhs, T rhs); \
}; \
extern template struct scalar_int<T>

    AUL_INTERNAL_EXPLICIT_DEC_SCALAR(float);
    AUL_INTERNAL_EXPLICIT_DEC_SCALAR(double);
    AUL_INTERNAL_EXPLICIT_DEC_SCALAR_INT(int32);
    AUL_INTERNAL_EXPLICIT_DEC_SCALAR_INT(uint32);
    AUL_INTERNAL_EXPLICIT_DEC_SCALAR_INT(int64);
    AUL_INTERNAL_EXPLICIT_DEC_SCALAR_INT(uint64);
    AUL_INTERNAL_EXPLICIT_DEC_SCALAR_INT(int16);
    AUL_INTERNAL_EXPLICIT_DEC_SCALAR_INT(uint16);
    AUL_INTERNAL_EXPLICIT_DEC_SCALAR_INT(int8);
    AUL_INTERNAL_EXPLICIT_DEC_SCALAR_INT(uint8);

#undef AUL_INTERNAL_EXPLICIT_DEC_SCALAR
#undef AUL_INTERNAL_EXPLICIT_DEC_SCALAR_INT

    inline float to_radians(float degrees) { return degrees * scalarf::DEG_TO_RAD; }
    inline double to_radians(double degrees) { return degrees * scalard::DEG_TO_RAD; }
    inline float to_degrees(float radians) { return radians * scalarf::RAD_TO_DEG; }
    inline double to_degrees(double radians) { return radians * scalard::RAD_TO_DEG; }
}