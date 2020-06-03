#pragma once

#include <aul/scalar_math.h>
#include <aul/macros.h>
#include <aul/impl/simd_math_types.h>

// Disable the warning about negating unsigned values - that it doesn't do anything is expected behavior.
// #TODO: Do the same for LLVM and GCC
#if AUL_MSVC
#pragma warning(disable:4146) 
#endif // AUL_MSVC

namespace aul
{
    template<typename T> struct vector2;
    template<typename T, typename U> struct vector2_int;

    template<typename T>
    wide_ostream& operator<< (wide_ostream& out, const vector2<T>& vec);

    template<typename T>
    mb_ostream& operator<< (mb_ostream& out, const vector2<T>& vec);

    template<typename T, typename U>
    wide_ostream& operator<< (wide_ostream& out, const vector2_int<T, U>& vec);

    template<typename T, typename U>
    mb_ostream& operator<< (mb_ostream& out, const vector2_int<T, U>& vec);

    // These two specialization keep the uint8 and int8 components from being interpreted as characters.
    template<>
    mb_ostream& operator<< (mb_ostream& out, const vector2_int<uint8, float>& vec);

    template<>
    mb_ostream& operator<< (mb_ostream& out, const vector2_int<int8, float>& vec);

    template<typename T>
    struct vector2
    {
        union
        {
            T data[2];
            struct
            {
                T x;
                T y;
            };
            struct  
            {
                T u;
                T v;
            };
        };

        static const vector2 ZERO;
        static const vector2 ONE;
        static const vector2 X_AXIS;
        static const vector2 Y_AXIS;

        vector2(T x_ = scalar<T>::ZERO, T y_ = scalar<T>::ZERO) : x(x_), y(y_) { }
        vector2(const T* arr_data);
        vector2& operator=(const T* arr_data);

        inline bool operator==(const vector2& rhs) const { return scalar<T>::equal(x, rhs.x) && scalar<T>::equal(y, rhs.y); }
        inline bool operator!=(const vector2& rhs) const { return scalar<T>::not_equal(x, rhs.x) || scalar<T>::not_equal(y, rhs.y); }
        inline vector2 operator-() const { return vector2(-x, -y); }
        inline vector2 operator+(const vector2& rhs) const { return vector2(x + rhs.x, y + rhs.y); }
        inline vector2 operator-(const vector2& rhs) const { return vector2(x - rhs.x, y - rhs.y); }
        inline vector2 operator*(T rhs) const { return vector2(x * rhs, y * rhs); }
        inline friend vector2 operator*(T lhs, const vector2<T>& rhs) { return vector2<T>(lhs * rhs.x, lhs * rhs.y); }
        inline vector2 operator/(T rhs) const { return vector2(x / rhs, y / rhs); }
        inline friend vector2 operator/(T lhs, const vector2<T>& rhs) { return vector2<T>(lhs / rhs.x, lhs / rhs.y); }
        inline vector2 operator*(const vector2& rhs) const { return vector2(x * rhs.x, y * rhs.y); }
        inline vector2 operator/(const vector2& rhs) const { return vector2(x / rhs.x, y / rhs.y); }
        inline vector2& operator+=(const vector2& rhs) { x += rhs.x; y += rhs.y; return *this; }
        inline vector2& operator-=(const vector2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
        inline vector2& operator*=(T rhs) { x *= rhs; y *= rhs; return *this; }
        inline vector2& operator/=(T rhs) { x /= rhs; y /= rhs; return *this; }
        inline vector2& operator*=(const vector2& rhs) { x *= rhs.x; y *= rhs.y; return *this; }
        inline vector2& operator/=(const vector2& rhs) { x /= rhs.x; y /= rhs.y; return *this; }

        inline T length() const { return sqrt(x * x + y * y); }
        inline T length_sq() const { return x * x + y * y; }
        vector2& normalize();
        vector2 normalized() const;
        inline T dot(const vector2& rhs) const { return x * rhs.x + y * rhs.y; }
        inline vector2 cross() const { return vector2(y, x); }
        vector2 project(const vector2& axis) const;

        operator wide_string() const;
        friend wide_ostream& operator<< <> (wide_ostream& out, const vector2<T>& vec);

        operator mb_string() const;
        friend mb_ostream& operator<< <> (mb_ostream& out, const vector2<T>& vec);
    };

    template<typename T, typename U>
    struct vector2_int
    {
        union
        {
            T data[2];
            struct
            {
                T x;
                T y;
            };
            struct  
            {
                T u;
                T v;
            };
        };

        static const vector2_int ZERO;
        static const vector2_int ONE;
        static const vector2_int X_AXIS;
        static const vector2_int Y_AXIS;

        vector2_int(T x_ = scalar_int<T>::ZERO, T y_ = scalar_int<T>::ZERO) : x(x_), y(y_) { }
        vector2_int(const T* arr_data);
        vector2_int& operator=(const T* arr_data);

        inline bool operator==(const vector2_int& rhs) const { return x == rhs.x && y == rhs.y; }
        inline bool operator!=(const vector2_int& rhs) const { return x != rhs.x || y != rhs.y; }
        inline vector2_int operator-() const { return vector2_int(-x, -y); }
        inline vector2_int operator+(const vector2_int& rhs) const { return vector2_int(x + rhs.x, y + rhs.y); }
        inline vector2_int operator-(const vector2_int& rhs) const { return vector2_int(x - rhs.x, y - rhs.y); }
        inline vector2_int operator*(T rhs) const { return vector2_int(x * rhs, y * rhs); }
        inline friend vector2_int operator*(T lhs, const vector2_int<T, U>& rhs) { return vector2_int<T, U>(lhs * rhs.x, lhs * rhs.y); }
        inline vector2_int operator/(T rhs) const { return vector2_int(x / rhs, y / rhs); }
        inline friend vector2_int operator/(T lhs, const vector2_int<T, U>& rhs) { return vector2_int<T, U>(lhs / rhs.x, lhs / rhs.y); }
        inline vector2_int operator*(const vector2_int& rhs) const { return vector2_int(x * rhs.x, y * rhs.y); }
        inline vector2_int operator/(const vector2_int& rhs) const { return vector2_int(x / rhs.x, y / rhs.y); }
        inline vector2_int& operator+=(const vector2_int& rhs) { x += rhs.x; y += rhs.y; return *this; }
        inline vector2_int& operator-=(const vector2_int& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
        inline vector2_int& operator*=(T rhs) { x *= rhs; y *= rhs; return *this; }
        inline vector2_int& operator/=(T rhs) { x /= rhs; y /= rhs; return *this; }
        inline vector2_int& operator*=(const vector2_int& rhs) { x *= rhs.x; y *= rhs.y; return *this; }
        inline vector2_int& operator/=(const vector2_int& rhs) { x /= rhs.x; y /= rhs.y; return *this; }

        inline U length() const { return sqrt((U)x * (U)x + (U)y * (U)y); }
        inline U length_sq() const { return (U)x * (U)x + (U)y * (U)y; }

        operator wide_string() const;
        friend wide_ostream& operator<< <> (wide_ostream& out, const vector2_int<T, U>& vec);

        operator mb_string() const;
        friend mb_ostream& operator<< <> (mb_ostream& out, const vector2_int<T, U>& vec);
    };

#define AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC(T, extension) typedef vector2<T> vector2 ## extension; \
extern template struct vector2<T>

#define AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT(T, U, extension) typedef vector2_int<T, U> vector2 ## extension; \
extern template struct vector2_int<T, U>

    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC(float, f);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC(double, d);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT(int32, float, i32);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT(uint32, float, u32);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT(int64, double, i64);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT(uint64, double, u64);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT(int16, float, i16);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT(uint16, float, u16);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT(int8, float, i8);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT(uint8, float, u8);

#undef AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC
#undef AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT

    //////////////////////////////////////////////////////////////////////////
    // Conversion
    //////////////////////////////////////////////////////////////////////////

#define AUL_INTERNAL_CONVERT_DEC_PARTIAL(T, U, V) template<> vector2<T>& convert<vector2<T>, vector2_int<U, V>>(vector2<T>& to, const vector2_int<U, V>& from); \
template<> vector2_int<U, V>& convert<vector2_int<U, V>, vector2<T>>(vector2_int<U, V>& to, const vector2<T>& from)

#define AUL_INTERNAL_CONVERT_DEC(T, U, V, W) AUL_INTERNAL_CONVERT_DEC_PARTIAL(T, V, W); \
AUL_INTERNAL_CONVERT_DEC_PARTIAL(U, V, W); \
AUL_INTERNAL_CONVERT_DEC_PARTIAL(T, u ## V, W); \
AUL_INTERNAL_CONVERT_DEC_PARTIAL(U, u ## V, W)

#define AUL_INTERNAL_CONVERT_DEC_INT(T, U, V, W) template<> vector2_int<T, U>& convert<vector2_int<T, U>, vector2_int<V, W>>(vector2_int<T, U>& to, const vector2_int<V, W>& from)

#define AUL_INTERNAL_CONVERT_DEC_INT_CROSS(T, U, V, W) AUL_INTERNAL_CONVERT_DEC_INT(T, U, V, W); \
AUL_INTERNAL_CONVERT_DEC_INT(u ## T, U, V, W); \
AUL_INTERNAL_CONVERT_DEC_INT(T, U, u ## V, W); \
AUL_INTERNAL_CONVERT_DEC_INT(u ## T, U, u ## V, W)

#define AUL_INTERNAL_CONVERT_DEC_INT_FAMILY(S, T, U, V, W, X, Y, Z) AUL_INTERNAL_CONVERT_DEC_INT(S, T, u ## S, T); \
AUL_INTERNAL_CONVERT_DEC_INT(u ## S, T, S, T); \
AUL_INTERNAL_CONVERT_DEC_INT_CROSS(S, T, U, V); \
AUL_INTERNAL_CONVERT_DEC_INT_CROSS(S, T, W, X); \
AUL_INTERNAL_CONVERT_DEC_INT_CROSS(S, T, Y, Z);

#define AUL_INTERNAL_CONVERT_DEC_FLOAT(T, U) template<> vector2<T>& convert<vector2<T>, vector2<U>>(vector2<T>& to, const vector2<U>& from); \
template<> vector2<U>& convert<vector2<U>, vector2<T>>(vector2<U>& to, const vector2<T>& from)

    AUL_INTERNAL_CONVERT_DEC(float, double, int32, float);
    AUL_INTERNAL_CONVERT_DEC(float, double, int64, double);
    AUL_INTERNAL_CONVERT_DEC(float, double, int16, float);
    AUL_INTERNAL_CONVERT_DEC(float, double, int8, float);

    AUL_INTERNAL_CONVERT_DEC_INT_FAMILY(int32, float, int64, double, int16, float, int8, float);
    AUL_INTERNAL_CONVERT_DEC_INT_FAMILY(int64, double, int32, float, int16, float, int8, float);
    AUL_INTERNAL_CONVERT_DEC_INT_FAMILY(int16, float, int32, float, int64, double, int8, float);
    AUL_INTERNAL_CONVERT_DEC_INT_FAMILY(int8, float, int32, float, int64, double, int16, float);

    AUL_INTERNAL_CONVERT_DEC_FLOAT(float, double);

#undef AUL_INTERNAL_CONVERT_DEC_FLOAT
#undef AUL_INTERNAL_CONVERT_DEC_INT_FAMILY
#undef AUL_INTERNAL_CONVERT_DEC_INT_CROSS
#undef AUL_INTERNAL_CONVERT_DEC_INT
#undef AUL_INTERNAL_CONVERT_DEC
#undef AUL_INTERNAL_CONVERT_DEC_PARTIAL

    // SIMD set and get
#if AUL_USE_SSE
    inline vec4f vec4f_set(const vector2f& vec2)
    {
        return _mm_set_ps(0.0f, 0.0f, vec2.y, vec2.x);
    }

    inline vector2f vec4f_get_vector2f(vec4f vec)
    {
        float vec_array[4];
        _mm_store_ps(vec_array, vec);
        return vector2f(vec_array[0], vec_array[1]);
    }
#endif // AUL_USE_SSE
}

#if AUL_MSVC
#pragma warning(default:4146)
#endif // AUL_MSVC